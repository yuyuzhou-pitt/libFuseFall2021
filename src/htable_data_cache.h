#ifndef _HTABLE_DATA_CACHE_H_
#define _HTABLE_DATA_CACHE_H_

struct htable_data_cache;
typedef struct htable_data_cache htable_data_cache_t;

struct htable_data_cache_enum;
typedef struct htable_data_cache_enum htable_data_cache_t;

htable_data_cache_t *htable_data_cache_create();
void htable_data_cache_destroy(htable_data_cache_t *ht);

void htable_data_cache_insert(htable_data_cache_t *ht, const char *key, void *val);
void htable_data_cache_remove(htable_data_cache_t *ht, const char *key);

int htable_data_cache_get(htable_data_cache_t *ht, const char *key, void **val);
void htable_data_cache_update(htable_data_cache_t *ht, const char *key, void *val);

htable_data_cache_t *htable_data_cache_enum_create(htable_data_cache_t *ht);
int htable_data_cache_enum_next(htable_data_cache_t *he, const char **key, void **val);
void htable_data_cache_enum_destroy(htable_data_cache_t *he);


#endif /*_HTABLE_DATA_CACHE_H_*/
