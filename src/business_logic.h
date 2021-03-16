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

/*
 * Changes the usage total for given user
 * uint64_t user_id, the inter id representing the user
 * int32_t total_change: total number of bytes to add to usage
 * returns 1 if successful, 0 if unsuccessful
 */
int add_usage_record(uint64_t user_id, int64_t total_change);

/*
 * Initializes the global database, pointing to a file.
 * const char *path: The file to create the database as. 
 */
int
db_init(const char* path);


#endif /*_BUSINESS_LOGIC_H_*/
