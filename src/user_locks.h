#ifndef _USER_LOCKS_H_
#define _USER_LOCKS_H_

#include <sys/types.h>

#include "htable_mutex.h"

/*
 * Initializes the user locks
 * uid_t *uid_arr: pointer to array of user ids to initialize
 * locks for
 * returns 0 if successful, 1 if not successful
 */
int init_user_locks(uid_t *uid_arr);

/*
 * Destroys the user locks
 */
void destroy_user_locks();

/*
 * Locks the lock associated with given user, makes lock if 
 * one doesn't already exist
 * uid_t uid: user to lock lock for
 * returns 0 if successful, 1 if not successful
 */
int lock_user_mutex(uid_t uid);

/*
 * Unlocks the lock associated with given user
 * uid_t uid: user to unlock lock for
 * returns 0 if successful, 1 if not successful
 */
int unlock_user_mutex(uid_t uid);

#endif /* _USER_LOCKS_H */
