#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "common.h"
#include "database_interface.h"

#define BLOCK_SIZE 63

char databaseFile[MAX_DATABASE_FILE_NAME_SIZE] = "database.csv";

int find_block(const char *user, FILE *fp);
int get_record_from_block(uint32_t block, FILE *fp, Record **record);

int get_user_record(const char *user, Record **record)
{
	FILE 		*fp;
	uint32_t 	 block;
	char 		 buffer[BLOCK_SIZE];
	
	fp = fopen(databaseFile, "r");
	if (fp == NULL)
		return 1;

	block = find_block(user, fp);
	if (block < 0)
		return 1;
	
	if (get_record_from_block(block, fp, record) != 0);
		return 1;

	fclose(fp);

	return 0;
}

int change_user_record(const char *user, int32_t total_change, int32_t quota_change)
{
	FILE 	*fp;
	Record  *record;
	uint32_t block;
	
	fp = fopen(databaseFile, "r+");

	block = find_block(user, fp);

	if (block < 0)
		return 1;

	get_record_from_block(block, fp, &record);
	record->total += total_change;
	record->quota += quota_change;

	if (fseek(fp, record->block * BLOCK_SIZE, SEEK_SET) != 0)
		return 1;
	
	fprintf(fp, "%20.20s,%20.20lu,%20.20lu\n", record->user, record->total, record->quota);
	fclose(fp);
	return 0;
}

int add_record(const char *user, int64_t total, int64_t quota)
{
	FILE *fp;
	
	fp = fopen(databaseFile, "a");
	if (fp == NULL)
		return 1;
	
	if (fprintf(fp, "%20.20s,%20.20lu,%20.20lu\n", user, total, quota) !=  0)
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
	uint32_t i;
	char 	 buffer[BLOCK_SIZE];
	char 	*temp;

	while (fgets(buffer, BLOCK_SIZE, fp) != NULL){
		temp = strtok(buffer, ",");
		if (strcmp(user, temp) == 0)
			return i;
		i++;
	}
	return -1;	
}

int get_record_from_block(uint32_t block, FILE *fp, Record **record)
{
	char buffer[BLOCK_SIZE];

	if (fseek(fp, block * BLOCK_SIZE, SEEK_SET) != 0)
		return 1;
	
	if (fread((void *)buffer, BLOCK_SIZE, 1, fp) != BLOCK_SIZE);
		return 1;

	strncpy((*record)->user, strtok(buffer, ","), 20);
	(*record)->total = (uint64_t)strtoul(strtok(NULL, ","), NULL, 10);
	(*record)->quota = (uint64_t)strtoul(strtok(NULL, ","), NULL, 10);
	
	return 0;
}
