#ifndef _BUSINESS_LOGIC_H_
#define _BUSINESS_LOGIC_H_

#include <sys/types.h>
#include <stdint.h>
/*
 * Checks if given user to allowed to do action depending
 * on whether they exceed their quota 
 * const chat *user: string name of user performing action
 * int size: needed size increase or decrease for action
 * returns 1 if action is allowed, 0 if not allowed
 */
int reserve_space (uid_t user_id, uint64_t reservation_size);

/*
 * Updates user total in database
 * const char *user: string name of user to change total of
 * int size: size increase or decrease of total
 * returns 0 if successful, 1 if not successful
 */
int update_reservation (uid_t user_id, uint64_t reservation_size, uint64_t num_used);

int db_int();

#endif /*_BUSINESS_LOGIC_H_*/
