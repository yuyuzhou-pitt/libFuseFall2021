#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/types.h>

#include "unity.h"
#include "htable.h"

htable *ht;
uint32_t *default_key;
uint32_t *default_val;
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
	ht = htable_create( hash, keq, NULL);
	
	default_key = (uint32_t *)malloc(sizeof(uint32_t));
	*default_key = 1234;

	default_val = (uint32_t *)malloc(sizeof(uint32_t));
	*default_val = 5678;

	he = htable_enum_create(ht);
}

void tearDown(void)
{
	htable_destroy(ht);
	htable_enum_destroy(he);
}

void make_hash_pair(uint32_t **key, uint32_t **val)
{
	srand(0);
	*key = (uint32_t *)malloc(sizeof(uint32_t));
	**key = (uint32_t)rand();

	*val = (uint32_t *)malloc(sizeof(uint32_t));
	**val = (uint32_t)rand();
}

void test_single_insert(void)
{
	htable_insert(ht, (void *)default_key, (void *)default_val);
	
	uint32_t *return_key;
	uint32_t *return_val;

	TEST_ASSERT_TRUE(htable_enum_next(he, (void **)&return_key, (void **)&return_val));
	
	TEST_ASSERT_EQUAL_UINT32(*default_key, *return_key);
	TEST_ASSERT_EQUAL_UINT32(*default_val, *return_val);
}

void test_double_insert(void)
{
	uint32_t *key;
	uint32_t *val;

	make_hash_pair(&key, &val);
	htable_insert(ht, (void *)key, (void *)val);
	htable_insert(ht, (void *)default_key, (void *)default_val);
	
	uint32_t *return_val1 = NULL;
	uint32_t *return_val2 = NULL;
	
	TEST_ASSERT_TRUE(htable_get(ht, (void *)key, (void **)&return_val1));
	TEST_ASSERT_TRUE(htable_get(ht, (void *)default_key, (void **)&return_val2));

	TEST_ASSERT_TRUE(return_val1 == val || return_val1 == default_val);
	TEST_ASSERT_FALSE(return_val1 == val && return_val1 == default_val);
	TEST_ASSERT_FALSE(return_val1 == return_val2);
}

void test_insert_update_existing_key(void)
{
	htable_insert(ht, (void *)default_key, (void *)default_val);

	uint32_t *val = (uint32_t *)malloc(sizeof(uint32_t));
	*val = 1111;

	htable_insert(ht, (void *)default_key, (void *)val);
	
	uint32_t * return_val = NULL;
	TEST_ASSERT_TRUE(htable_get(ht, (void *)default_key, (void **)&return_val));
	TEST_ASSERT_EQUAL_INT(1111, *return_val);
}

void test_null_insert(void)
{
	htable_insert(NULL, (void *)default_key, (void *)default_val);
	TEST_ASSERT_FALSE(htable_get(ht, (void *)default_key, NULL));	

	htable_insert(ht, NULL, (void *)default_val);
	TEST_ASSERT_FALSE(htable_enum_next(he, NULL, NULL));
}

void test_single_remove()
{
	htable_insert(ht, (void *)default_key, (void *)default_val);

	htable_remove(ht, (void *)default_key);
	TEST_ASSERT_FALSE(htable_get(ht, (void *)default_key, NULL));
}

void test_removes_only_specified_key_and_val()
{
	uint32_t *key;
	uint32_t *val;

	make_hash_pair(&key, &val);
	htable_insert(ht, (void *)key, (void *)val);
	htable_insert(ht, (void *)default_key, (void *)default_val);

	htable_remove(ht, (void *)default_key);
	TEST_ASSERT_FALSE(htable_get(ht, (void *)default_key, NULL));

	TEST_ASSERT_TRUE(htable_get(ht, (void *)key, NULL));
}

void test_null_remove()
{
	htable_insert(ht, (void *)default_key, (void *)default_val);

	htable_remove(NULL, (void *)default_key);
	TEST_ASSERT_TRUE(htable_get(ht, (void *)default_key, NULL));
	
	htable_remove(ht, NULL);
	TEST_ASSERT_TRUE(htable_get(ht, (void *)default_key, NULL));
}

void test_get_value(void)
{
	htable_insert(ht, (void *)default_key, (void *)default_val);
	
	uint32_t *return_val = NULL;

	TEST_ASSERT_TRUE(htable_get(ht, (void *)default_key, (void **)&return_val));
	TEST_ASSERT_TRUE(return_val == default_val);
}

void test_get_false_on_nonexistent_key(void)
{
	uint32_t key = 9999;
	TEST_ASSERT_FALSE(htable_get(ht, (void *)&key, NULL));

	htable_insert(ht, (void *)default_key, (void *)default_val);
	key = 5555;
	
	TEST_ASSERT_FALSE(htable_get(ht, (void *)&key, NULL));
}

void test_null_get(void)
{
	TEST_ASSERT_FALSE(htable_get(NULL, (void *)default_key, NULL));
	TEST_ASSERT_FALSE(htable_get(ht, NULL, NULL));
}
