#include <time.h>
#include <stdlib.h>
#include <limits.h>
#include <stdint.h>

#include "htable.h"

struct htable_bucket {
	void *key;
	void *val;
	struct htable_bucket *next;
};
typedef struct htable_bucket htable_bucket_t;

struct htable {
	htable_hash	 hfunc;
	htable_keq	 keq;
	htable_cbs	 cbs;
	htable_bucket_t *buckets;
	size_t 		 num_buckets;
	size_t 		 num_used;
	unsigned int     seed;
};

struct htable_enum {
	htable_t	*ht;
	htable_bucket_t *cur;
	size_t		 idx;
};

static const size_t BUCKET_START = 16;

void rehash(htable_t *ht);
void htable_add_to_bucket(htable_t *ht, void *key, void *val, int isrehash);

static size_t htable_bucket_idx(htable_t *ht, void *key)
{
	return ht->hfunc(key, ht->seed) % ht->num_buckets;
}

static void *htable_passthrough_copy(void *v)
{
	return v;
}

static void htable_passthrough_destroy(void *v)
{
	return;
}

htable_t *htable_create(htable_hash hfunc, htable_keq keq, htable_cbs *cbs)
{
	htable_t *ht;

	if (hfunc == NULL || keq == NULL)
		return NULL;

	ht = calloc(1,sizeof(*ht));

	ht->hfunc = hfunc;
	ht->keq = keq;

	ht->cbs.key_copy = htable_passthrough_copy;
	ht->cbs.key_free = htable_passthrough_destroy;
	ht->cbs.val_copy = htable_passthrough_copy;
	ht->cbs.val_free = htable_passthrough_destroy;
	if (cbs != NULL){
		if (cbs->key_copy != NULL) ht->cbs.key_copy = cbs->key_copy;
		if (cbs->key_free != NULL) ht->cbs.key_free = cbs->key_free;
		if (cbs->val_copy != NULL) ht->cbs.val_copy = cbs->val_copy;
		if (cbs->val_free != NULL) ht->cbs.val_free = cbs->val_free;
	}

	ht->num_buckets = BUCKET_START;
	ht->buckets = calloc(BUCKET_START, sizeof(*ht->buckets));
	
	ht->seed = (unsigned int)time(NULL);
	ht->seed ^= ((uintptr_t)htable_create << 16) | (uintptr_t)ht;
	ht->seed ^= (uintptr_t)&ht->seed;

	return ht;
}

void htable_destroy(htable_t *ht)
{
	htable_bucket_t *next;
	htable_bucket_t *cur;
	size_t 		 i;

	if (ht == NULL)
		return;

	for (i=0; i < ht->num_buckets; i++){
		if (ht->buckets[i].key == NULL)
			continue;	
		ht->cbs.key_free(ht->buckets[i].key);
		ht->cbs.val_free(ht->buckets[i].val);
	
		next = ht->buckets[i].next;
		while (next != NULL) {
			cur = next;
			ht->cbs.key_free(ht->buckets[i].key);
			ht->cbs.val_free(ht->buckets[i].val);
			next = cur->next;
			free(cur);
		}
	}

	free(ht->buckets);
	free(ht);
}

void htable_insert(htable_t *ht, void *key, void *val)
{
	if (ht == NULL || key == NULL)
		return;

	rehash(ht);
	htable_add_to_bucket(ht, key, val, 0);
}

void rehash(htable_t *ht)
{
	htable_bucket_t *buckets;
	htable_bucket_t *cur;
	htable_bucket_t *next;
	size_t 		 num_buckets;
	size_t		 i;

	if (ht->num_used+1 < (size_t)(ht->num_buckets*0.75) || ht->num_buckets >= 1<<31)
		return;

	buckets = ht->buckets;
	num_buckets = ht->num_buckets;
	ht->num_buckets <<= 1;
	ht->buckets = calloc(ht->num_buckets, sizeof(*ht->buckets));
	
	for(i=0; i < num_buckets; i++){
		if (buckets[i].key == NULL)
			continue;
		htable_add_to_bucket(ht, buckets[i].key, buckets[i].val, 1);

		if (buckets[i].next != NULL){
			cur = buckets[i].next;
			do {
				htable_add_to_bucket(ht, cur->key, cur->val, 1);
				next = cur-> next;
				free(cur);
				cur = next;
			} while (cur != NULL);
		}
	}

	free(buckets);
}

void htable_add_to_bucket(htable_t *ht, void *key, void *val, int isrehash)
{
	htable_bucket_t *cur;
	htable_bucket_t *prev;
	size_t		 idx;

	idx = htable_bucket_idx(ht, key);
	if (ht->buckets[idx].key == NULL){
		if (!isrehash) {
			key = ht->cbs.key_copy(key);
			if (val != NULL)
				val = ht->cbs.val_copy(val);
		}
		ht->buckets[idx].key = key;
		ht->buckets[idx].val = val;
		if (!isrehash)
			ht->num_used++;
	} else {
		cur = ht->buckets+idx;		// pointer arithmetic
		prev = ht->buckets+idx;
		do {
			if (ht->keq(key, cur->key)){
				if (cur->val != NULL)
					ht->cbs.val_free(cur->val);
				if (!rehash && val != NULL)
					val = ht->cbs.val_copy(val);
				cur->val = val;
				prev = NULL;
				break;
			}
			prev = cur;
			cur = cur->next;
		} while (cur != NULL);
		
		if (prev != NULL) {
			cur = calloc(1, sizeof(*cur->next));	
			if (!isrehash) {
				key = ht->cbs.key_copy(key);
				if (val != NULL)
					val = ht->cbs.val_copy(val);
			}
			cur->key = key;
			cur->val = val;
			prev->next = cur;
			if (!isrehash)
				ht->num_used++;
		}
	}
}
void htable_remove(htable_t *ht, void *key)
{
	htable_bucket_t *cur;
	htable_bucket_t *prev;
	size_t 		 idx;
	
	if (ht == NULL || key == NULL)
		return;
	
	idx = htable_bucket_idx(ht, key);
	if (ht->buckets[idx].key == NULL)
		return;

	if (ht->keq(ht->buckets[idx].key, key)){
		ht->cbs.key_free(ht->buckets[idx].key);
		ht->cbs.val_free(ht->buckets[idx].val);
		ht->buckets[idx].key = NULL;

		cur = ht->buckets[idx].next;
		if (cur != NULL) {
			ht->buckets[idx].key = cur->key;
			ht->buckets[idx].val = cur->val;
			ht->buckets[idx].next = cur->next;
			free(cur);
		}
		ht->num_used--;
		return;
	}

	prev = ht->buckets+idx;
	cur = prev->next;
	while (cur != NULL) {
		if (ht->keq(cur->key, key)){
			ht->cbs.key_free(cur->key);
			ht->cbs.val_free(cur->val);
			prev->next = cur->next;
			free(cur);
			ht->num_used--;
			break;
		}
		prev = cur;
		cur = cur->next;
			
	}
}

int htable_get(htable_t *ht, void *key, void **val)
{
	htable_bucket_t *cur;
	size_t		 idx;

	if ( ht == NULL || key == NULL)
		return 0;

	idx = htable_bucket_idx(ht, key);
	if (ht->buckets[idx].key == NULL)
		return 0;

	cur = ht->buckets+idx;
	while (cur != NULL) {
		if (ht->keq(cur->key, key)){
			if (val != NULL)
				*val = cur->val;
			return 1;
		}
		cur = cur->next;
	}

	return 0;
}

void htable_update(htable_t *ht, void *key, void *val){
	htable_bucket_t *cur;
	size_t		 idx;

	if (ht == NULL || key == NULL)
		return;

	idx = htable_bucket_idx(ht, key);
	if(ht->buckets[idx].key == NULL)
		return;

	cur = ht->buckets+idx;
	while (cur != NULL) {
		if (ht->keq(cur->key, key)){
			if (cur->val != NULL)
				ht->cbs.val_free(cur->val);
			if ( val != NULL)
				val = ht->cbs.val_copy(val);
			cur->val = val;
			return;
		}
		cur = cur->next;
	}
}

htable_enum_t *htable_enum_create(htable_t *ht)
{
    htable_enum_t *he;

    if (ht == NULL)
        return NULL;

    he = calloc(1, sizeof(*he));
    he->ht = ht;

    return he;
}

int htable_enum_next(htable_enum_t *he, void **key, void **val)
{
    void *mykey;
    void *myval;

    if (he == NULL || he->idx >= he->ht->num_buckets)
        return 0;

    if (key == NULL)
        key = &mykey;
    if (val == NULL)
        val = &myval;

    if (he->cur == NULL) {
        while (he->idx < he->ht->num_buckets && he->ht->buckets[he->idx].key == NULL) {
            he->idx++;
        }
        if (he->idx >= he->ht->num_buckets)
            return 0;
        he->cur = he->ht->buckets+he->idx;
        he->idx++;
    }

    *key = he->cur->key;
    *val = he->cur->val;
    he->cur = he->cur->next;

    return 1;
}

void htable_enum_destroy(htable_enum_t *he)
{
	if (he == NULL)
		return;
	free(he);
}
