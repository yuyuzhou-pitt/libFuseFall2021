#ifndef _HTABLE_RECORD_CACHE_H_
#define _HTABLE_RECORD_CACHE_H_

#include <stdbool.h>
#include <pthread.h>
#include <sys/types.h>

struct htable_mutex;
typedef struct htable_mutex htable_mutex;

struct htable_mutex_enum;
typedef struct htable_mutex_enum htable_mutex_enum;

htable_mutex *htable_mutex_create();
void htable_mutex_destroy(htable_mutex *ht);

void htable_mutex_insert(htable_mutex *ht, uid_t *key, pthread_mutex_t *val);
void htable_mutex_remove(htable_mutex *ht, uid_t *key);

bool htable_mutex_get(htable_mutex *ht, uid_t *key, pthread_mutex_t **val);
void htable_mutex_update(htable_mutex *ht, uid_t *key, pthread_mutex_t *val);

htable_mutex_enum *htable_mutex_enum_create(htable_mutex *ht);
bool htable_mutex_enum_next(htable_mutex_enum *he, uid_t **key, pthread_mutex_t **val);
void htable_mutex_enum_destroy(htable_mutex_enum *he);


#endif /*_HTABLE_RECORD_CACHE_H_*/
