#ifndef _BUSINESS_LOGIC_H_
#define _BUSINESS_LOGIC_H_

#include <sys/types.h>
#include <stdint.h>
/*
 * Checks if given user to allowed to do action depending
 * on whether they exceed their quota
 * uint64_t user_id: id number of the user
 * int reservation_size: the number of bytes to reserve
 * returns 1 if action is allowed, 0 if not allowed
 */
int reserve_space (uid_t user_id, uint64_t reservation_size);

/*
 * Updates user byte total in database.
 * 
 * uid_t user_id: id number of the user
 * int64_t byte_total_changed: amount to change user byte total by
 * returns 0 if successful, 1 if not successful
 */
int update_user_total (uid_t user_id, int64_t byte_total_changed);

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
