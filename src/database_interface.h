#ifndef _DATABASE_INTERFACE_H_
#define _DATABASE_INTERFACE_H_

#include <stdint.h>

#include "common.h"

#define MAX_DATABASE_FILE_NAME_SIZE 20

/*
 * Returns a user record
 * const char *user: user to retrieve record for
 * record **record: return record pointer
 * returns 0 if successful, 1 if unsuccessful 
 */
int get_user_record(const char *user, Record **record);

/*
 * Changes the user total for given user
 * const char *user: string name of user to change total of
 * int32_t total_change: change of total amount
 * int32_t quota_change: change of quota amount
 * returns 0 if successful, 1 if unsuccessful
 */
int change_user_record(const char *user, int32_t total_change, int32_t quota_change);

/*
 * Adds a record to the database
 * const char *user: string name of user
 * int64_t total: total bytes used by user
 * int64_t quota: quota of user
 * returns 0 if successful, 1 if unsuccessful
 */
int add_record(const char *user, int64_t total, int64_t quota);

/*
 * Changes database target file
 * const char *file_name: file name of new target file, must be less than
 * MAX_DATABASE_FILE_NAME_SIZE
 * returns 0 if successful, 1 if unsuccessful
 */
int change_databaseFile(const char *file_name);

#endif /* _DATABASE_INTERFACE_H_*/
