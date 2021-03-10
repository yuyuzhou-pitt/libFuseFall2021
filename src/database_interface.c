/* TODO
 * 1. Make sure it works
 * 2. Testing edge cases to make sure all strings are null-terminated
 * 3. Add some sort of concurrency protection. I(Greg) has some ideas tha may or may not work.
 * 4. Add caching
 * 5. Maybe binary search on file as a stretch goal
 */

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "common.h"
#include "database_interface.h"

#define BLOCK_SIZE 63

char databaseFile[MAX_DATABASE_FILE_NAME_SIZE] = "database.csv";

int find_block(const char *user, FILE *fp);
int get_record_from_block(uint32_t block, FILE *fp, Record *record);
void strip_whitespace(char *string);

int get_user_record(const char *user, Record *record)
{
	FILE 		*fp;
	uint32_t 	 block;
	
	fp = fopen(databaseFile, "r");
	if (fp == NULL)
		return 1;
	printf("file opened\n");
	block = find_block(user, fp);
	if (block < 0)
		return 1;
	printf("block found: %u\n", block);
	if (get_record_from_block(block, fp, record) != 0)
		return 1;
	printf("record retrieved\n");
	fclose(fp);

	return 0;
}

int change_user_record(const char *user, int32_t total_change, int32_t quota_change)
{
	FILE 	*fp;
	Record   record;
	uint32_t block;
	
	fp = fopen(databaseFile, "r+");

	block = find_block(user, fp);

	if (block < 0)
		return 1;

	get_record_from_block(block, fp, &record);
	record.total += total_change;
	record.quota += quota_change;

	if (fseek(fp, record.block * BLOCK_SIZE, SEEK_SET) != 0)
		return 1;
	
	if (fprintf(fp, "%-20.20s,%20.20lu,%20.20lu\n", record.user, record.total, record.quota) != BLOCK_SIZE)
		return 1;
	fclose(fp);
	return 0;
}

int add_record(const char *user, int64_t total, int64_t quota)
{
	FILE *fp;
	
	fp = fopen(databaseFile, "a");
	if (fp == NULL)
		return 1;
	
	if (fprintf(fp, "%-20.20s,%20.20lu,%20.20lu\n", user, total, quota) != BLOCK_SIZE)
		return 1;
	
	fclose(fp);
	return 0;
}

int change_databaseFile(const char *file_name)
{
	if (strlen(file_name) >= MAX_DATABASE_FILE_NAME_SIZE)
		return 1;
	
	strncpy(databaseFile, file_name, MAX_DATABASE_FILE_NAME_SIZE);
	return 0;
}

int find_block(const char *user, FILE *fp)
{
	uint32_t i = 0;
	char 	 buffer[BLOCK_SIZE+1];
	char 	*temp;
	printf("Username to find: %s\n", user);
	while (fgets(buffer, BLOCK_SIZE+1, fp) != NULL){
		temp = strtok(buffer, ",");
		strip_whitespace(temp);
		printf("Current user:%s block:%u\n", temp, i);
		if (strcmp(user, temp) == 0)
			return i;
		i++;
	}
	return -1;	
}

int get_record_from_block(uint32_t block, FILE *fp, Record *record)
{
	char  buffer[BLOCK_SIZE];
	char *temp;
	char *strtok_save_ptr;	

	if (fseek(fp, block * BLOCK_SIZE, SEEK_SET) != 0)
		return 1;
	//printf("bytes read:%li\n", fread(buffer, 1, BLOCK_SIZE, fp));
	if (fread(buffer, 1, BLOCK_SIZE, fp) != BLOCK_SIZE)
		return 1;
	printf("line read:%s", buffer);
	temp = strtok_r(buffer, ",", &strtok_save_ptr);
	strip_whitespace(temp);
	printf("Stripped temp:%s\n", temp);
	strncpy(record->user, temp, sizeof(record->user));
	printf("username copied\n");
	record->total = (uint64_t)strtoul(strtok_r(NULL, ",", &strtok_save_ptr), NULL, 10);
	record->quota = (uint64_t)strtoul(strtok_r(NULL, ",", &strtok_save_ptr), NULL, 10);
	record->block = block;
	return 0;
}

void strip_whitespace(char *string)
{
	int i;
	for(i = 0; i < sizeof(string); i++){
		if (string[i] ==  ' '){
			string[i] = '\0';
			return;
		}
	}
}
