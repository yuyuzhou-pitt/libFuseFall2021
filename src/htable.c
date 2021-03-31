/*
		Copyright John Schember <john@nachtimwald.com>

	Permission is hereby granted, free of charge, to any person obtaining a copy of
	this software and associated documentation files (the "Software"), to deal in
	the Software without restriction, including without limitation the rights to
	use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
	of the Software, and to permit persons to whom the Software is furnished to do
	so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.
*/

#include <time.h>
#include <stdlib.h>
#include <limits.h>
#include <stdint.h>
#include <stdbool.h>

#include "htable.h"

struct htable_bucket {
	void *key;
	void *val;
	struct htable_bucket *next;
};
typedef struct htable_bucket htable_bucket;

struct htable {
	htable_hash	 hfunc;
	htable_keq	 keq;
	htable_cbs	 cbs;
	htable_bucket *buckets;
	size_t 		 num_buckets;
	size_t 		 num_used;
	unsigned int     seed;
};

struct htable_enum {
	htable	*ht;
	htable_bucket *cur;
	size_t		 idx;
};

static const size_t BUCKET_START = 16;

void rehash(htable *ht);
void htable_add_to_bucket(htable *ht, void *key, void *val, bool isrehash);

static size_t htable_bucket_idx(htable *ht, void *key)
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

htable *htable_create(htable_hash hfunc, htable_keq keq, htable_cbs *cbs)
{
	htable *ht;

	if (hfunc == NULL || keq == NULL)
		return NULL;

	ht = calloc(1, sizeof(*ht));

	ht->hfunc = hfunc;
	ht->keq = keq;

	ht->cbs.key_copy = htable_passthrough_copy;
	ht->cbs.key_free = htable_passthrough_destroy;
	ht->cbs.val_copy = htable_passthrough_copy;
	ht->cbs.val_free = htable_passthrough_destroy;
	if (cbs != NULL) {
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

void htable_destroy(htable *ht)
{
	htable_bucket *next;
	htable_bucket *cur;
	size_t 	       i;

	if (ht == NULL)
		return;

	for (i=0; i < ht->num_buckets; i++) {
		if (ht->buckets[i].key == NULL)
			continue;	
		
		ht->cbs.key_free(ht->buckets[i].key);
		ht->cbs.val_free(ht->buckets[i].val);
		
		next = ht->buckets[i].next;
		while (next != NULL) {
			cur = next;
			ht->cbs.key_free(cur->key);
			ht->cbs.val_free(cur->val);
			next = cur->next;
			free(cur);
		}
	}

	free(ht->buckets);
	free(ht);
}

void htable_insert(htable *ht, void *key, void *val)
{
	if (ht == NULL || key == NULL)
		return;

	rehash(ht);
	htable_add_to_bucket(ht, key, val, false);
}

void rehash(htable *ht)
{
	htable_bucket *buckets;
	htable_bucket *cur;
	htable_bucket *next;
	size_t 	       num_buckets;
	size_t	       i;

	if (ht->num_used+1 < (size_t)(ht->num_buckets*0.75) || ht->num_buckets >= 1<<31)
		return;

	buckets = ht->buckets;
	num_buckets = ht->num_buckets;
	ht->num_buckets <<= 1;
	ht->buckets = calloc(ht->num_buckets, sizeof(*buckets));
	
	for(i=0; i < num_buckets; i++) {
		if (buckets[i].key == NULL)
			continue;

		htable_add_to_bucket(ht, buckets[i].key, buckets[i].val, true);
		if (buckets[i].next != NULL) {
			cur = buckets[i].next;
			do {
				htable_add_to_bucket(ht, cur->key, cur->val, true);
				next = cur-> next;
				free(cur);
				cur = next;
			} while (cur != NULL);
		}
	}

	free(buckets);
}

void htable_add_to_bucket(htable *ht, void *key, void *val, bool isrehash)
{
	htable_bucket *cur;
	htable_bucket *prev;
	size_t	       idx;

	idx = htable_bucket_idx(ht, key);
	if (ht->buckets[idx].key == NULL) {
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
				if (!isrehash && val != NULL)
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

void htable_remove(htable *ht, void *key)
{
	htable_bucket *cur;
	htable_bucket *prev;
	size_t 	       idx;
	
	if (ht == NULL || key == NULL)
		return;
	
	idx = htable_bucket_idx(ht, key);
	if (ht->buckets[idx].key == NULL)
		return;

	if (ht->keq(ht->buckets[idx].key, key)) {
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

bool htable_get(htable *ht, void *key, void **val)
{
	htable_bucket *cur;
	size_t	       idx;

	if ( ht == NULL || key == NULL)
		return false;

	idx = htable_bucket_idx(ht, key);
	if (ht->buckets[idx].key == NULL)
		return false;

	cur = ht->buckets+idx;
	while (cur != NULL) {
		if (ht->keq(key, cur->key)) {
			if (val != NULL)
				*val = cur->val;
			return true;
		}
		cur = cur->next;
	}

	return false;
}

void htable_update(htable *ht, void *key, void *val){
	htable_bucket *cur;
	size_t	       idx;

	if (ht == NULL || key == NULL)
		return;

	idx = htable_bucket_idx(ht, key);
	if(ht->buckets[idx].key == NULL)
		return;

	cur = ht->buckets+idx;
	while (cur != NULL) {
		if (ht->keq(cur->key, key)) {
			if (cur->val == val)
				return;
			if (cur->val != NULL)
				ht->cbs.val_free(cur->val);
			if (val != NULL)
				val = ht->cbs.val_copy(val);
			cur->val = val;
			return;
		}
		cur = cur->next;
	}
}

htable_enum *htable_enum_create(htable *ht)
{
    htable_enum *he;

    if (ht == NULL)
        return NULL;

    he = calloc(1, sizeof(*he));
    he->ht = ht;

    return he;
}

bool htable_enum_next(htable_enum *he, void **key, void **val)
{
    void *mykey;
    void *myval;

    if (he == NULL || he->idx >= he->ht->num_buckets)
        return false;

    if (key == NULL)
        key = &mykey;
    if (val == NULL)
        val = &myval;

    if (he->cur == NULL) {
        while (he->idx < he->ht->num_buckets && he->ht->buckets[he->idx].key == NULL) {
            he->idx++;
        }
        if (he->idx >= he->ht->num_buckets)
            return false;
        he->cur = he->ht->buckets+he->idx;
        he->idx++;
    }

    *key = he->cur->key;
    *val = he->cur->val;
    he->cur = he->cur->next;

    return true;
}

void htable_enum_destroy(htable_enum *he)
{
	if (he == NULL)
		return;
	free(he);
}
