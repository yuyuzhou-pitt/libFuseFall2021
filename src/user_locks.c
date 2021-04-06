#include "user_locks.h"
#include "common.h"
#include <stdlib.h>
#include <stdio.h>



int init_user_locks(uid_t *uid_arr) {
	//insert all locks into hash table
	user_locks_htable = htable_mutex_create();
	int i;
	int length = (int)uid_arr[0];
	if(length < 0) {
		free(uid_arr);
		return 0;
	}
	for(i = 1;i <= length;i++) {
		pthread_mutex_t *lock;
		insert_new_mutex(uid_arr[i], &lock);
	}
	free(uid_arr);
	return 0;
}

void insert_new_mutex(uid_t uid, pthread_mutex_t **lock) {
	uid_t *key = (uid_t *)malloc(sizeof(uid_t));
	*key = uid;
	
	*lock = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(*lock, NULL);
	
	
	htable_mutex_insert(user_locks_htable, key, *lock);
}

void destroy_user_locks() {
	struct htable_mutex_enum *enumerator = htable_mutex_enum_create(user_locks_htable);
	pthread_mutex_t *return_thread = NULL;
	
	bool cont = htable_mutex_enum_next(enumerator, NULL, &return_thread);
	while(cont) {
		pthread_mutex_destroy(return_thread);
		
		cont = htable_mutex_enum_next(enumerator, NULL, &return_thread);
	}
	
	htable_mutex_enum_destroy(enumerator);
	htable_mutex_destroy(user_locks_htable);
}

int lock_user_mutex(uid_t uid) {
	pthread_mutex_t *lock;
	
	if(!htable_mutex_get(user_locks_htable, &uid, &lock)) {
		insert_new_mutex(uid, &lock);
	}
	
	return pthread_mutex_lock(lock);
}

int unlock_user_mutex(uid_t uid) {
	pthread_mutex_t *lock;
	
	if(!htable_mutex_get(user_locks_htable, &uid, &lock)) {
		return 1;
	}
	
	return pthread_mutex_unlock(lock);
}