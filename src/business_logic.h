#ifndef business_logic.h
#define business_logic.h

/*
 * Checks if given user to allowed to do action depending
 * on whether they exceed their quota 
 * const chat *user: string name of user performing action
 * int size: needed size increase or decrease for action
 * returns 1 if action is allowed, 0 if not allowed
 */
int check_action (const char *user, int size);

/*
 * Updates user total in database
 * const char *user: string name of user to change total of
 * int size: size increase or decrease of total
 * returns 1 if successful, 0 if not successful
 */
int update_user_total (const char *user, int size);

#endif
