#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>

#include <limits.h>

#include "common.h"
#include "database.h"
#include "htable_record_cache.h"

#define BLOCK_SIZE 105
#define WRITE_TO_FILE_THRESHOLD 500

char databaseFile[MAX_DATABASE_FILE_NAME_SIZE] = "/../database.csv";
htable_record_cache *ht = NULL;
uint state_operations_since_last_write = 0;

/*
 * Creates a hashtable database from a database file
 * uid_t *users: array to be filled with all users in the database, first element is size
 * returns standard return values
 */
int create_database_from_file(uid_t *users);

/*
 * Writes the current hashtable database out to databasefile
 * returns standard return values
 */
int write_database_to_file();

/*
 * Checks if enough state changing database operations have happened since
 * the last write to file to warrent another write. WRITE_TO_FILE_THRESHOLD
 * defines what that number is.
 * returns standard return values
 */
int check_need_to_write_to_file();

int get_user_record(uid_t user_id, Record *record)
{
	if (htable_record_cache_get(ht, &user_id, &record))
		return 0;
	return 3;
}

int change_user_record(uid_t user_id, int64_t byte_total_change, int64_t byte_quota_change, int64_t file_total_change, int64_t file_quota_change)
{
	if (check_need_to_write_to_file() != 0)
		return 1;

	Record *record = (Record *)malloc(sizeof(Record));

	if (!htable_record_cache_get(ht, &user_id, &record))
		return 3;
	
	record->byte_total = ((record->byte_total + byte_total_change) < 0) ? 0 : (record->byte_total + byte_total_change);
	record->byte_quota = ((record->byte_quota + byte_quota_change) < 0) ? 0 : (record->byte_quota + byte_quota_change);
	record->file_total = ((record->file_total + file_total_change) < 0) ? 0 : (record->file_total + file_total_change);
	record->file_quota = ((record->file_quota + file_quota_change) < 0) ? 0 : (record->file_quota + file_quota_change);
	
	htable_record_cache_update(ht, &user_id, record);
	state_operations_since_last_write++;
	return 0;
}

int add_user_record(uid_t user_id, uint64_t byte_total, uint64_t byte_quota, uint64_t file_total, uint64_t file_quota)
{
	if (check_need_to_write_to_file() != 0)
		return 1;
	
	Record *record = (Record *)malloc(sizeof(Record));
	uid_t *user_key = (uid_t *)malloc(sizeof(uid_t));

	if (htable_record_cache_get(ht, &user_id, NULL))
		return 4;

	record->user_id = user_id;
	record->byte_total = byte_total;
	record->byte_quota = byte_quota;
	record->file_total = file_total;
	record->file_quota = file_quota;
	*user_key = user_id;

	htable_record_cache_insert(ht, user_key, record);
	state_operations_since_last_write++;
	return 0;
}

int print_all_records()
{
	Record *record;
	htable_record_cache_enum *he = htable_record_cache_enum_create(ht);

	while (htable_record_cache_enum_next(he, NULL, &record)) {
		printf("User_ID:%-20i Byte_total:%-20lu Byte_quota:%-20lu File_total:%-20lu File_quota:%-20lu\n", record->user_id, record->byte_total, record->byte_quota, record->file_total, record->file_quota);
	}
	htable_record_cache_enum_destroy(he);
	
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
	if (ht != NULL) {
		write_database_to_file();
		htable_record_cache_destroy(ht);
	}

	ht = htable_record_cache_create();
	return 0;
}

uid_t * database_init()
{
	char   file_path[PATH_MAX];
	struct stat sb;

	create_empty_database();
	
	fpath(databaseFile, file_path);
	if (stat(file_path, &sb) != 0) {
		uid_t *users = (uid_t *)malloc(sizeof(uid_t));
		*users = -1;
		return users;
	}
	uid_t *users = (uid_t *)malloc(sizeof(uid_t)*((sb.st_size/BLOCK_SIZE)+1));

	if (create_database_from_file(users) != 0) 
		users[0] = -1;
	
	return users;
}

int database_close()
{
	int ret = write_database_to_file();	

	htable_record_cache_destroy(ht);
	
	return ret;
}

int create_database_from_file(uid_t *users)
{
	FILE  *fp;
	char   file_path[PATH_MAX];
	char   buffer[BLOCK_SIZE+1];
	char  *temp;
	char  *strtok_save_ptr;	
	int    i = 0;

	fpath(databaseFile, file_path);
	fp = fopen(file_path, "r");
	if (fp == NULL)
		return 2;	

	while (fgets(buffer, BLOCK_SIZE+1, fp) != NULL) {
		Record *record = (Record *)malloc(sizeof(Record));
		uid_t  *user_key = (uid_t *)malloc(sizeof(uid_t));	
		i++;
	
		temp = strtok_r(buffer, ",", &strtok_save_ptr);
		record->user_id = (uid_t)atoi(temp);
		record->byte_total = (uint64_t)strtoul(strtok_r(NULL, ",", &strtok_save_ptr), NULL, 10);
		record->byte_quota = (uint64_t)strtoul(strtok_r(NULL, ",", &strtok_save_ptr), NULL, 10);
		record->file_total = (uint64_t)strtoul(strtok_r(NULL, ",", &strtok_save_ptr), NULL, 10);
		record->file_quota = (uint64_t)strtoul(strtok_r(NULL, ",", &strtok_save_ptr), NULL, 10);
		htable_record_cache_insert(ht, user_key, record);
		users[i] = record->user_id;
	}
	users[0] = i;

	return 0;	
}

int write_database_to_file()
{
	FILE   *fp;
	char    file_path[PATH_MAX];
	Record *record;
	int     ret_val = 0;

	fpath(databaseFile, file_path);
	fp = fopen(file_path, "w");
	if (fp == NULL)
		return 2;	

	htable_record_cache_enum *he = htable_record_cache_enum_create(ht);

	while (htable_record_cache_enum_next(he, NULL, &record)) {
		if (fprintf(fp, "%20.20i,%20.20lu,%20.20lu,%20.20lu,%20.20lu\n", record->user_id, record->byte_total, record->byte_quota, record->file_total, record->file_quota) != BLOCK_SIZE)
			ret_val = 1;
	}
	htable_record_cache_enum_destroy(he);
	
	fclose(fp);

	return ret_val;
}

int check_need_to_write_to_file()
{
	if (state_operations_since_last_write >= WRITE_TO_FILE_THRESHOLD) {
		state_operations_since_last_write == 0;
		return write_database_to_file();
	}
	return 0;
}
