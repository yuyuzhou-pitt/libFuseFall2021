#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>

#include "htable.h"
#include "htable_mutex.h"

static unsigned int fnv1a_hash_uid(const void *in, unsigned int seed);
static int  htable_uid_eq(const void *a, const void *b);

htable_mutex *htable_mutex_create()
{
	htable_hash hash = fnv1a_hash_uid;
	htable_keq keq = htable_uid_eq;
	htable_cbs cbs = {
		NULL,
		free,
		NULL,
		free
	};
	
	return (htable_mutex *)htable_create(hash, keq, &cbs);
}

void htable_mutex_destroy(htable_mutex *ht)
{
	htable_destroy((htable *)ht);
}

void htable_mutex_insert(htable_mutex *ht, uid_t *key, pthread_mutex_t *val)
{
	htable_insert((htable *)ht, (void *)key, (void *)val);
}

void htable_mutex_remove(htable_mutex *ht, uid_t *key)
{
	htable_remove((htable *)ht, (void *)key);
}

bool htable_mutex_get(htable_mutex *ht, uid_t *key, pthread_mutex_t **val)
{
	return htable_get((htable *)ht, (void *)key, (void *)val);
}

void htable_mutex_update(htable_mutex *ht, uid_t *key, pthread_mutex_t *val)
{
	htable_update((htable *)ht, (void *)key, (void *)val);
}

htable_mutex_enum *htable_mutex_enum_create(htable_mutex *ht)
{
	return (htable_mutex_enum *)htable_enum_create((htable *)ht);
}

bool htable_mutex_enum_next(htable_mutex_enum *he, uid_t **key, pthread_mutex_t **val)
{
	return htable_enum_next((htable_enum *)he, (void **)key, (void **)val);
}

void htable_mutex_enum_destroy(htable_mutex_enum *he)
{
	htable_enum_destroy((htable_enum *)he);
}

static unsigned int fnv1a_hash_uid(const void *in, unsigned int seed)
{
	unsigned int h = seed;
	unsigned int c;
	size_t	     i;

	for (i=0; i < sizeof(in); i++) {
		c = ((unsigned char *)in)[i];
		h ^= c;
		h *= 16777619;
	}

	return h;
}

static int htable_uid_eq(const void *a, const void *b){
	return *(uid_t *)a == *(uid_t *)b;
}
