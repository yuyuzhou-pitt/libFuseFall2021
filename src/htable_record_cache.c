#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "htable.h"
#include "htable_record_cache.h"
#include "common.h"

static unsigned int fnv1a_hash_str(const void *in, unsigned int seed);
static int htable_str_eq(const void *a, const void *b);

htable_record_cache *htable_record_cache_create()
{
	htable_hash hash = fnv1a_hash_str;
	htable_keq keq = htable_str_eq;
	htable_cbs cbs = {
		(void *(*)(void *))strdup,
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

void htable_record_cache_insert(htable_record_cache *ht, const char *key, Record *val)
{
	htable_insert((htable *)ht, (void *)key, (void *)val);
}

void htable_record_cache_remove(htable_record_cache *ht, const char *key)
{
	htable_remove((htable *)ht, (void *)key);
}

bool htable_record_cache_get(htable_record_cache *ht, const char *key, Record **val)
{
	return htable_get((htable *)ht, (void *)key, (void *)val);
}

void htable_record_cache_update(htable_record_cache *ht, const char *key, Record *val)
{
	htable_update((htable *)ht, (void *)key, (void *)val);
}

htable_record_cache_enum *htable_record_cache_enum_create(htable_record_cache *ht)
{
	return (htable_record_cache_enum *)htable_enum_create((htable *)ht);
}

bool htable_record_cache_enum_next(htable_record_cache_enum *he, const char **key, Record **val)
{
	return htable_enum_next((htable_enum *)he, (void **)key, (void **)val);
}

void htable_record_cache_enum_destroy(htable_record_cache_enum *he)
{
	htable_enum_destroy((htable_enum *)he);
}

static unsigned int fnv1a_hash_str_int(const void *in, size_t len, unsigned int seed)
{
	unsigned int h = seed;
	unsigned int c;
	size_t	     i;

	for (i=0; i < len; i++) {
		c = ((const unsigned char *)in)[i];
	h ^= c;
	h *= 16777619;
	}

	return h;
}

static unsigned int fnv1a_hash_str(const void *in, unsigned int seed)
{
	return fnv1a_hash_str_int(in, strlen((const char *)in), seed);
}

static int htable_str_eq(const void *a, const void *b){
	return (strcmp(a, b) == 0) ? 1 : 0;
}
