#ifndef _DATABASE_H_
#define _DATABASE_H_

#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h> 

#include <stdint.h>

#define MAX_DATABASE_FILE_NAME_SIZE 20

/*
 * Returns a user record
 * uint64_t user_id, the inter id representing the user
 * returns 1 if successful, 0 if unsuccessful 
 */
int contains_user(uint64_t user_id);

/*
 * Changes the usage total for given user
 * uint64_t user_id, the inter id representing the user
 * int32_t total_change: total number of bytes to add to usage
 * returns 1 if successful, 0 if unsuccessful
 */
int add_usage_record(uint64_t user_id, int32_t total_change);

/*
 * Changes the file total for given user
 * uint64_t user_id, the inter id representing the user
 * int32_t total_change: total number of files to add to the record
 * returns 1 if successful, 0 if unsuccessful
 */
int add_file_record(uint64_t user_id, int32_t total_change);

/*
 * Deletes all values from the database.
 */
void clear_db();

/*
 * Prints every value in the database to console.
 */
void print_all();

/*
 * Adds a record to the database
 * uint64_t user_id, the inter id representing the user
 * returns 0 if successful, corresponsing SQLite error code if unsuccessful
 */
int add_user(uint64_t user_id);

/*
 * Initializes the global database, pointing to a file.
 * const char *path: The file to create the database as. 
 */
int
db_init(const char* path);

/*
 * Closes the global database.
 */
int
db_close();

#endif /* _DATABASE_H_*/