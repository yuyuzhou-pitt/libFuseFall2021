#ifndef _DATABASE_H_
#define _DATABASE_H_

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>                                                           
#include <sys/types.h>
#include <sys/stat.h>

#include "common.h"

#define MAX_DATABASE_FILE_NAME_SIZE 25

/*
 * Default database filename is 'database.csv'
 *
 * Return values:
 */
 #define DB_SUCCESS             0    /* everything okay                  */
 #define DB_ERROR               1    /* error occurred                   */
 #define DB_NOT_INIT            2    /* database does not exist          */
 #define DB_USER_NOT_EXIST      3    /* user does not exist in database  */
 #define DB_USER_ALREADY_EXIST  4    /* user already exists in database  */

/*
 * Returns a user record
 * uid_t user_id: user id to retrieve record for
 * record *record: return record pointer
 * returns standard return value
 */
int get_user_record(uid_t user_id, Record **record);

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
 * Creates an empty database. If a database already exists, will write
 * it to file before deleting it and making a new one.
 * Returns standard return values
 */
int create_empty_database();

/*
 * Initializes database
 * returns a dynamically allocated uid_t array of all users in the database with the first element
 * being the size of the array
 */
uid_t * database_init();

/*
 * Writes current database to file then closes it.
 * returns standard return values
 */
int database_close();

#endif /* _DATABASE_H_*/
