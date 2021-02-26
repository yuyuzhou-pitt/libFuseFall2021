#ifndef _DATABASE_INTERFACE_H_
#define _DATABASE_INTERFACE_H_

/* 
 * Returns total number of bytes used by given user
 * const char *user: string name of user to get total of
 * returns a number of bytes used, -1 if error occured
 */
long user_total(const char *user);


/* 
 * Returns user quota in bytes
 * const char *user: string name of user to get quota of
 * returns user byte quota, -1 if error occured
 */
long user_quota(const char *user);


/*
 * Changes the user total for given user
 * const char *user: string name of user to change total of
 * int size: size increase or decrease of user total
 * returns 1 if successful, 0 if unsuccessful
 */
int change_user_total(const char *user, int size);

#endif /* _DATABASE_INTERFACE_H_*/
