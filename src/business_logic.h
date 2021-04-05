#ifndef _BUSINESS_LOGIC_H_
#define _BUSINESS_LOGIC_H_

#include <sys/types.h>

/*
 * Changes the usage total for given user
 * uint64_t user_id, the inter id representing the user
 * int32_t total_change: total number of bytes to add to usage
 * returns 1 if successful, 0 if unsuccessful
 */
int update_usage_record(uid_t user_id, int64_t total_change);

/*
 * Changes the usage total for given user
 * uint64_t user_id, the inter id representing the user
 * int32_t total_change: total number of bytes to add to usage
 * returns 1 if successful, 0 if unsuccessful
 */
int update_file_record(uid_t user_id, int64_t total_change);

/*
 * Prints every value in the database to console.
 */
void print_all();

/*
 * Initializes the database
 * returns a uid_t array of users in database with the first element
 * being the number of users
 */
uid_t * db_init();

/*
 * Closes the database
 * returns standard return values
 */
int db_close();


#endif /*_BUSINESS_LOGIC_H_*/
