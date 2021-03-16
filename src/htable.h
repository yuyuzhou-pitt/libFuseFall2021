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

#ifndef _HTABLE_H_
#define _HTABLE_H_

#include <stdbool.h>

struct htable;
typedef struct htable htable;

struct htable_enum;
typedef struct htable_enum htable_enum;

// Function Callbacks for passed in functions
typedef unsigned int (*htable_hash)(const void *in, unsigned int seed);
typedef int (*htable_keq)(const void *a, const void *b);
typedef void *(*htable_kcopy)(void *in);
typedef void (*htable_kfree)(void *in);
typedef void *(*htable_vcopy)(void *in);
typedef void (*htable_vfree)(void *in);

typedef struct {
	htable_kcopy key_copy;
	htable_kfree key_free;
	htable_vcopy val_copy;
	htable_vfree val_free;
} htable_cbs;

/*
 * Creates a new hash table
 * htable_hash hfunc: hashing function
 * htable_keq keq: key equality function
 * htable cbs *cbs: additional callback functions
 * returns htable pointer to created hash table
 */
htable *htable_create(htable_hash hfunc, htable_keq keq, htable_cbs *cbs);
/* 
 * Destroys hash table
 * htable *ht: htable pointer to hash table to destroy
 */
void htable_destroy(htable *ht);

/*
 * Insert key-value pair into hashtable
 * htable *ht: hashtable to be inserted into
 * void *key: key
 * void *val: value
 */
void htable_insert(htable *ht, void *key, void *val);

/*
 * Removes entry from hashtable
 * htable *ht: hashtable to remove from
 * void *key: key of entry to remove
 */
void htable_remove(htable *ht, void *key);

/*
 * Gets value associated with given key 
 * htable *ht: hashtable to retrieve from
 * void *key: key
 * void **val: return pointer to value associated with key
 * returns false if key not in hashtable, true if key is
 */
bool htable_get(htable *ht, void *key, void **val);

/* 
 * Updates value of key in hashtable
 * htable *ht: hashtable
 * void *key: key
 * void *value: new value
 */
void htable_update(htable *ht, void *key, void *val);

// Methods for enumerating through key-value pairs in hashtable
htable_enum *htable_enum_create(htable *ht);
bool htable_enum_next(htable_enum *he, void **key, void **val);
void htable_enum_destroy(htable_enum *he);

#endif /*_HTABLE_H_*/
