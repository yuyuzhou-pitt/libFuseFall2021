#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/types.h>

#include "unity.h"
#include "htable.h"

htable *ht;
uint32_t *key;
uint32_t *val;
htable_enum *he;

static unsigned int hash_func(const void *in, unsigned int seed)
{
	srand(*(unsigned int *)in + seed);
	return (unsigned int)rand();
}

static int key_eq(const void *a, const void *b)
{
	return *(uint32_t *)a == *(uint32_t *)b;
}

void setUp(void)
{ 
	htable_hash hash = hash_func;
	htable_keq keq = key_eq;
	ht = htable_record_cache_create( hash, keq, NULL);
	
	key = (uint32_t *)malloc(sizeof(uint32_t));
	*key = 1234;

	val = (uint32_t *)malloc(sizeof(uint32_t));
	*val = 5678;

	he = htable_record_cache_enum_create(ht);
}

void tearDown(void)
{
	htable_record_cache_destroy(ht);
	htable_record_cache_enum_destroy(he);
}

void test_insert(void)
{
	htable_record_cache_insert(ht, (void *)key, (void *)val);
	
	uint32_t *return_key;
	uint32_t *return_val;

	htable_record_cache_enum_next(he, (void **)&return_key, (void **)&return_val);
	
	TEST_ASSERT_EQUAL_UINT(*key, *return_key);
	TEST_ASSERT_EQUAL_UINT64(*val, *return_val);
}
