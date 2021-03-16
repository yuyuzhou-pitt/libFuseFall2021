#ifndef _DATABASE_H_
#define _DATABASE_H_

#include <stdint.h>

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
 * Changes the user total for given user
 * uid_t user_id: user id to change total of
 * int32_t total_change: change of total amount
 * int32_t quota_change: change of quota amount
 * returns standard return value
 */
int change_user_record(uid_t user_id, int32_t total_change, int32_t quota_change);

/*
 * Adds a new user record to the database, will not add user record if record
 * alread exists for given user if
 * uid_t user_id: string name of user
 * int64_t total: total bytes used by user
 * int64_t quota: quota of user
 * returns standard return values
 */
int add_user_record(uid_t user_id, int64_t total, int64_t quota);

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
