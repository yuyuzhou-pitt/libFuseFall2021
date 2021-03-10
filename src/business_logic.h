#ifndef _BUSINESS_LOGIC_H_
#define _BUSINESS_LOGIC_H_

/*
 * Checks if given user to allowed to do action depending
 * on whether they exceed their quota 
 * const chat *user: string name of user performing action
 * int size: needed size increase or decrease for action
 * returns 1 if action is allowed, 0 if not allowed
 */
int check_action (uint32_t  user, int size_change);

/*
 * Updates user total in database
 * const char *user: string name of user to change total of
 * int size: size increase or decrease of total
 * returns 0 if successful, 1 if not successful
 */
int update_user_total (uint32_t  user, int total_change);

#endif /*_BUSINESS_LOGIC_H_*/
