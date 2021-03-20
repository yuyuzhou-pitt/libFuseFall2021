/* TODO
 * 1. Convert over to hash table
 * 2. Add some sort of checking to make sure database is correct by scanning filesystem
 */

/*
 * Database design
 * The database is a flat file stored on disk. In this instance it is a csv normally located in 
 * 'database.csv'. Each record in the database is an exact byte length defined as BLOCK_SIZE with
 * each record taking up one "block" in the database. If you know the block a record is located in
 * you can fseek( block * BLOCK_SIZE ) and read in the desired data. The purpose of the standardized  * blocks is to enable updating records in place with needing to make new files every time.
 */

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>

#include "common.h"
#include "database.h"

#define BLOCK_SIZE 105

char databaseFile[MAX_DATABASE_FILE_NAME_SIZE] = "database.csv";

/*
 * Finds the block(line) of database file user record is in, can
 * also be used as a user record existence check
 * uid_t user_id: user_id of record to find
 * FILE *fp: file pointer to already opened file
 * returns number of block(0+) user record lives in, -1 if not found
 */
int find_block_of_user(uid_t user_id, FILE *fp);

/*
 * Fills in a record with information from database
 * int32_t block: database block desired information lives in, should be zero or greater
 * Record *record: pointer to Record struct that is filled with data
 * returns standard return values
 */
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

int change_user_record(uid_t user_id, int64_t byte_total_change, int64_t byte_quota_change, int64_t file_total_change, int64_t file_quota_change)
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
	record.byte_total = ((record.byte_total + byte_total_change) < 0) ? 0 : (record.byte_total + byte_total_change);
	record.byte_quota = ((record.byte_quota + byte_quota_change) < 0) ? 0 : (record.byte_quota + byte_quota_change);
	record.file_total = ((record.file_total + file_total_change) < 0) ? 0 : (record.file_total + file_total_change);
	record.file_quota = ((record.file_quota + file_quota_change) < 0) ? 0 : (record.file_quota + file_quota_change);

	if (fseek(fp, record.block * BLOCK_SIZE, SEEK_SET) != 0)
		return 1;
	
	if (fprintf(fp, "%20.20i,%20.20lu,%20.20lu,%20.20lu,%20.20lu\n", record.user_id, record.byte_total, record.byte_quota, record.file_total, record.file_quota) != BLOCK_SIZE)
		return 1;
	fclose(fp);
	return 0;
}

int add_user_record(uid_t user_id, uint64_t byte_total, uint64_t byte_quota, uint64_t file_total, uint64_t file_quota)
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
	
	if (fprintf(fp, "%20.20i,%20.20lu,%20.20lu,%20.20lu,%20.20lu\n", user_id, byte_total, byte_quota, file_total, file_quota) != BLOCK_SIZE)
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
		record.byte_total = (uint64_t)strtoul(strtok_r(NULL, ",", &strtok_save_ptr), NULL, 10);
		record.byte_quota = (uint64_t)strtoul(strtok_r(NULL, ",", &strtok_save_ptr), NULL, 10);
		record.file_total = (uint64_t)strtoul(strtok_r(NULL, ",", &strtok_save_ptr), NULL, 10);
		record.file_quota = (uint64_t)strtoul(strtok_r(NULL, ",", &strtok_save_ptr), NULL, 10);
		printf("User_ID:%-20i Byte_total:%-20lu Byte_quota:%-20lu File_total:%-20lu File_quota:%-20lu\n", record.user_id, record.byte_total, record.byte_quota, record.file_total, record.file_quota);
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

int database_init()
{
	FILE *fp;

	fp = fopen(databaseFile, "r");
	if (fp == NULL)
		return create_empty_database();
	return 0;	
}

int database_close()
{
	return 0;
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
	
	temp = strtok_r(buffer, ",", &strtok_save_ptr);
	record->user_id = (uid_t)atoi(temp);
	record->byte_total = (uint64_t)strtoul(strtok_r(NULL, ",", &strtok_save_ptr), NULL, 10);
	record->byte_quota = (uint64_t)strtoul(strtok_r(NULL, ",", &strtok_save_ptr), NULL, 10);
	record->file_total = (uint64_t)strtoul(strtok_r(NULL, ",", &strtok_save_ptr), NULL, 10);
	record->file_quota = (uint64_t)strtoul(strtok_r(NULL, ",", &strtok_save_ptr), NULL, 10);
	record->block = block;
	return 0;
}
