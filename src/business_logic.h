#ifndef _BUSINESS_LOGIC_H_
#define _BUSINESS_LOGIC_H_

/*
 * Checks if given user to allowed to do action depending
 * on whether they exceed their quota .
 * uint64_t user_id: id number of the user
 * int reservation_size: the number of bytes to reserve
 * returns 1 if action is allowed, 0 if not allowed
 */
int reserve_space (uint64_t user_id, int reservation_size);

/*
 * Updates user total in database.
 * This shouls always be called after reserve_space()
 * 
 * uint64_t user_id: id number of the user
 * int reservation_size: the number of bytes reserved
 * int num_used: the number of bytes actually written
 * returns 0 if successful, 1 if not successful
 */
int update_reservation (uint64_t user_id, int reservation_size, int num_used);

#endif /*_BUSINESS_LOGIC_H_*/
