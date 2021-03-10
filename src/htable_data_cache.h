#ifndef _HTABLE_DATA_CACHE_H_
#define _HTABLE_DATA_CACHE_H_

#include <stdbool.h>

#include "common.h"

struct htable_data_cache;
typedef struct htable_data_cache htable_data_cache;

struct htable_data_cache_enum;
typedef struct htable_data_cache_enum htable_data_cache_enum;

htable_data_cache *htable_data_cache_create();
void htable_data_cache_destroy(htable_data_cache *ht);

void htable_data_cache_insert(htable_data_cache *ht, const char *key, Record *val);
void htable_data_cache_remove(htable_data_cache *ht, const char *key);

bool htable_data_cache_get(htable_data_cache *ht, const char *key, Record **val);
void htable_data_cache_update(htable_data_cache *ht, const char *key, Record *val);

htable_data_cache_enum *htable_data_cache_enum_create(htable_data_cache *ht);
bool htable_data_cache_enum_next(htable_data_cache_enum *he, const char **key, Record **val);
void htable_data_cache_enum_destroy(htable_data_cache_enum *he);


#endif /*_HTABLE_DATA_CACHE_H_*/
