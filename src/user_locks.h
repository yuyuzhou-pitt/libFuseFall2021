#include "htable_mutex.h"

htable_mutex *user_locks_htable;

int init_user_locks(int *uid_arr);
void destroy_user_locks();
int lock_user_mutex(int uid);
int unlock_user_mutex(int uid);