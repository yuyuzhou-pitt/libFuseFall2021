/* TODO
 * 1. Testing edge cases to make sure all strings are null-terminated
 * 2. Add some sort of concurrency protection. I(Greg) has some ideas tha may or may not work.
 * 3. Add caching
 * 4. Maybe binary search on file as a stretch goal
 */

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "common.h"
#include "database.h"

#define BLOCK_SIZE 63

char databaseFile[MAX_DATABASE_FILE_NAME_SIZE] = "database.csv";

int find_block_of_user(uid_t user_id, FILE *fp);
int get_record_from_block(int32_t block, FILE *fp, Record *record);

int get_user_record(uid_t user_id, Record *record)
{
	FILE 	*fp;
	int32_t  block;
	int	 return_value;
	
	fp = fopen(databaseFile, "r");
	if (fp == NULL)
		return 2;
	block = find_block_of_user(user_id, fp);
	if (block < 0)
		return 3;
	return_value = get_record_from_block(block, fp, record);
	if (return_value != 0)
		return return_value;
	fclose(fp);

	return 0;
}

int change_user_record(uid_t user_id, int32_t total_change, int32_t quota_change)
{
	FILE 	*fp;
	Record   record;
	int32_t block;
	
	fp = fopen(databaseFile, "r+");
	if (fp == NULL)
		return 2;

	block = find_block_of_user(user_id, fp);
	if (block < 0)
		return 3;

	if (get_record_from_block(block, fp, &record) != 0)
		return 1;
	record.total += total_change;
	record.quota += quota_change;

	if (fseek(fp, record.block * BLOCK_SIZE, SEEK_SET) != 0)
		return 1;
	
	if (fprintf(fp, "%20.20i,%20.20lu,%20.20lu\n", record.user_id, record.total, record.quota) != BLOCK_SIZE)
		return 1;
	fclose(fp);
	return 0;
}

int add_user_record(uid_t user_id, int64_t total, int64_t quota)
{
	FILE *fp;
	
	fp = fopen(databaseFile, "r");
	if (fp == NULL)
		return 2;
	if (find_block_of_user(user_id, fp) >= 0)
		return 4;
	fclose(fp);

	fp = fopen(databaseFile, "a+");
	if (fp == NULL)
		return 2;
	
	if (fprintf(fp, "%20.20i,%20.20lu,%20.20lu\n", user_id, total, quota) != BLOCK_SIZE)
		return 1;
	
	fclose(fp);
	return 0;
}

int print_all_records()
{
	FILE  *fp;
	char   buffer[BLOCK_SIZE+1];
	char  *temp;
	char  *strtok_save_ptr;	
	Record record;

	fp = fopen(databaseFile, "r");
	if (fp == NULL)
		return 2;	

	while (fgets(buffer, BLOCK_SIZE+1, fp) != NULL) {
		temp = strtok_r(buffer, ",", &strtok_save_ptr);
		record.user_id = (uid_t)atoi(temp);
		record.total = (uint64_t)strtoul(strtok_r(NULL, ",", &strtok_save_ptr), NULL, 10);
		record.quota = (uint64_t)strtoul(strtok_r(NULL, ",", &strtok_save_ptr), NULL, 10);
		printf("User ID:%i Total:%lu Quota:%lu\n", record.user_id, record.total, record.quota);
	}
	return 0;
}

int change_databaseFile(const char *file_name)
{
	if (strlen(file_name) >= MAX_DATABASE_FILE_NAME_SIZE)
		return 1;
	
	strncpy(databaseFile, file_name, MAX_DATABASE_FILE_NAME_SIZE);
	return 0;
}

int create_empty_database()
{
	FILE *fp;

	fp = fopen(databaseFile, "w");
	if (fp == NULL)
		return 1;

	fclose(fp);
	return 0	;
}

int find_block_of_user(uid_t user_id, FILE *fp)
{
	uint32_t i = 0;
	char 	 buffer[BLOCK_SIZE+1];
	char 	*temp;
	char	*strtok_save_ptr;

	while (fgets(buffer, BLOCK_SIZE+1, fp) != NULL){
		temp = strtok_r(buffer, ",", &strtok_save_ptr);
		if (user_id == atoi(temp))
			return i;
		i++;
	}
	return -1;	
}

int get_record_from_block(int32_t block, FILE *fp, Record *record)
{
	char  buffer[BLOCK_SIZE+1];
	char *temp;
	char *strtok_save_ptr;	

	if (fseek(fp, block * BLOCK_SIZE, SEEK_SET) != 0)
		return 1;
	if (fread(buffer, 1, BLOCK_SIZE, fp) != BLOCK_SIZE)
		return 1;
	buffer[BLOCK_SIZE] = '\0';
	//printf("line read:%s", buffer);
	
	temp = strtok_r(buffer, ",", &strtok_save_ptr);
	record->user_id = (uid_t)atoi(temp);
	//printf("user_idname copied\n");
	record->total = (uint64_t)strtoul(strtok_r(NULL, ",", &strtok_save_ptr), NULL, 10);
	record->quota = (uint64_t)strtoul(strtok_r(NULL, ",", &strtok_save_ptr), NULL, 10);
	record->block = block;
	return 0;
}
