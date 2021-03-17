#ifndef _DATABASE_H_
#define _DATABASE_H_

#include <stdint.h>
#include <sys/types.h>

#include "common.h"

#define MAX_DATABASE_FILE_NAME_SIZE 20

/*
 * Default database filename is 'database.csv'
 *
 * Return values:
 * 0: everything okay
 * 1: error occurred
 * 2: database does not exist
 * 3: user does not exist in database
 * 4: user already exists in database
 */

/*
 * Returns a user record
 * uid_t user_id: user id to retrieve record for
 * record *record: return record pointer
 * returns standard return value
 */
int get_user_record(uid_t user_id, Record *record);

/*
 * Changes user totals and quotas
 * uid_t user_id: user id to change total of
 * int32_t byte_total_change: change of byte total amount
 * int32_t byte_quota_change: change of byte quota amount
 * int32_t file_total_change: change of file total amount
 * int32_t file_quota_change: change of quota total amount
 * returns standard return value
 */
int change_user_record(uid_t user_id, int64_t byte_total_change, int64_t byte_quota_change, int64_t file_total_change, int64_t file_quota_change);

/*
 * Adds a new user record to the database, will not add user record if record
 * alread exists for given user if
 * uid_t user_id: string name of user
 * uint64_t byte total: total bytes used by user
 * uint64_t byte quota: byte quota of user
 * uint64_t file total: total files owned by user
 * uint64_t file quota: file quota of user
 * returns standard return values
 */
int add_user_record(uid_t user_id, uint64_t byte_total, uint64_t byte_quota, uint64_t file_total, uint64_t file_quota);

/*
 * Prints all records in the database
 * returns standard return values
 */
int print_all_records();

/*
 * Changes database target file
 * const char *file_name: file name of new target file, must be less than
 * MAX_DATABASE_FILE_NAME_SIZE
 * returns standard return values
 */
int change_databaseFile(const char *file_name);

/*
 * Creates an empty database from set database target file, will erase
 * existing database with the same filename
 * Returns standard return values
 */
int create_empty_database();

/*
 * Initializes database
 * returns standard return values
 */
int database_init();

/*
 * Closes database
 * returns standard return values
 */
int database_close();

#endif /* _DATABASE_H_*/
