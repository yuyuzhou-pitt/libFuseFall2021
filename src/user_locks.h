#include "htable_mutex.h"

htable_mutex *user_locks_htable;

int init_user_locks(uid_t *uid_arr);
void destroy_user_locks();
int lock_user_mutex(uid_t uid);
int unlock_user_mutex(uid_t uid);
void insert_new_mutex(uid_t uid, pthread_mutex_t **lock);