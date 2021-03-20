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
 * Updates user total in database.
 * This shouls always be called after reserve_space()
 * 
 * uid_t user_id: id number of the user
 * uint64_t reservation_size: the number of bytes reserved
 * uint64_t num_used: the number of bytes actually written
 * returns 0 if successful, 1 if not successful
 */
int update_reservation (uid_t user_id, uint64_t reservation_size, uint64_t num_used);

/*
 * Changes the usage total for given user
 * uid_t user_id, the inter id representing the user
 * int64_t total_change: number of bytes to change user bytes used by
 * returns 0 if successful, 1 if unsuccessful
 */
int add_usage_record(uid_t user_id, int64_t total_change);

/*
 * Initializes the database
 */
int db_int();

#endif /*_BUSINESS_LOGIC_H_*/
