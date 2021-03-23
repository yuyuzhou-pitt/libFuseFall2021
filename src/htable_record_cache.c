#include <stdlib.h>
#include <stdbool.h>

#include "htable.h"
#include "htable_record_cache.h"
#include "common.h"

static unsigned int fnv1a_hash_uid(const void *in, unsigned int seed);
static int  htable_uid_eq(const void *a, const void *b);

htable_record_cache *htable_record_cache_create()
{
	htable_hash hash = fnv1a_hash_uid;
	htable_keq keq = htable_uid_eq;
	htable_cbs cbs = {
		NULL,
		free,
		NULL,
		free
	};
	
	return (htable_record_cache *)htable_create(hash, keq, &cbs);
}

void htable_record_cache_destroy(htable_record_cache *ht)
{
	htable_destroy((htable *)ht);
}

void htable_record_cache_insert(htable_record_cache *ht, uid_t *key, Record *val)
{
	htable_insert((htable *)ht, (void *)key, (void *)val);
}

void htable_record_cache_remove(htable_record_cache *ht, uid_t *key)
{
	htable_remove((htable *)ht, (void *)key);
}

bool htable_record_cache_get(htable_record_cache *ht, uid_t *key, Record **val)
{
	return htable_get((htable *)ht, (void *)key, (void *)val);
}

void htable_record_cache_update(htable_record_cache *ht, uid_t *key, Record *val)
{
	htable_update((htable *)ht, (void *)key, (void *)val);
}

htable_record_cache_enum *htable_record_cache_enum_create(htable_record_cache *ht)
{
	return (htable_record_cache_enum *)htable_enum_create((htable *)ht);
}

bool htable_record_cache_enum_next(htable_record_cache_enum *he, uid_t **key, Record **val)
{
	return htable_enum_next((htable_enum *)he, (void **)key, (void **)val);
}

void htable_record_cache_enum_destroy(htable_record_cache_enum *he)
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
