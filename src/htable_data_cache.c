#include <stdlib.h>
#include <string.h>

#include "htable.h"
#include "htable_data_cache.h"
#include "common.h"

static unsigned int fnv1a_hash_str(const void *in, unsigned int seed);
static int htable_str_eq(const void *a, const void *b);

htable_data_cache_t *htable_data_cache_create()
{
	htable_hash hash = fnv1a_hash_str;
	htable_keq keq = htable_str_eq;
	htable_cbs cbs = {
		(void *(*)(void *))strdup,
		free,
		NULL,
		free
	};
	
	return (htable_data_cache_t *)htable_create(hash, keq, &cbs);
}

void htable_data_cache_destroy(htable_data_cache_t *ht)
{
	htable_destroy((htable_t *)ht);
}

void htable_data_cache_insert(htable_data_cache_t *ht, const char *key, Record *val)
{
	htable_insert((htable_t *)ht, (void *)key, (void *)val);
}

void htable_data_cache_remove(htable_data_cache_t *ht, const char *key)
{
	htable_remove((htable_t *)ht, (void *)key);
}

int htable_data_cache_get(htable_data_cache_t *ht, const char *key, Rcord **val)
{
	return htable_get((htable_t *)ht, (void *)key, (void *)val);
}

void htable_data_cache_update(htable_data_cache_t *ht, const char *key, Record *val)
{
	htable_update((htable_t *)ht, (void *)key, (void *)val);
}

htable_data_cache_t *htable_data_cache_enum_create(htable_data_cache_t *ht)
{
	return (htable_data_cache_enum_t *)htable_enum_create((htable_t *)ht);
}

int htable_data_cache_enum_next(htable_data_cache_t *he, const char **key, Record **val)
{
	return htable_enum_next((htable_enum_t *)he, (void **)key, (void **)val;
}

void htable_data_cache_enum_destroy(htable_data_cache_t *he)
{
	htable_enum_destroy((htable_enum_t *)he);
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
