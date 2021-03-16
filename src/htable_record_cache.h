#ifndef _HTABLE_RECORD_CACHE_H_
#define _HTABLE_RECORD_CACHE_H_

#include <stdbool.h>

#include "common.h"

struct htable_record_cache;
typedef struct htable_record_cache htable_record_cache;

struct htable_record_cache_enum;
typedef struct htable_record_cache_enum htable_record_cache_enum;

htable_record_cache *htable_record_cache_create();
void htable_record_cache_destroy(htable_record_cache *ht);

void htable_record_cache_insert(htable_record_cache *ht, const char *key, Record *val);
void htable_record_cache_remove(htable_record_cache *ht, const char *key);

bool htable_record_cache_get(htable_record_cache *ht, const char *key, Record **val);
void htable_record_cache_update(htable_record_cache *ht, const char *key, Record *val);

htable_record_cache_enum *htable_record_cache_enum_create(htable_record_cache *ht);
bool htable_record_cache_enum_next(htable_record_cache_enum *he, const char **key, Record **val);
void htable_record_cache_enum_destroy(htable_record_cache_enum *he);


#endif /*_HTABLE_RECORD_CACHE_H_*/
