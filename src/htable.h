#ifndef __HTABLE.H__
#define __HTABLE.H__

struct htable;
typedef struct htable htable_t;

struct htable_enum;
typedef struct htable_enum htable_enum_t;

// Function Callbacks for passed in functions
typedef unsigned int (*htable_hash)(const void *in, unsigned int seed);
typedef void *(*htable_kcopy)(void *in)
typedef bool (*htable_keq)(const void *a, const void *b);
typedef void (*htable_kfree)(void *in);
typedef void *(htable_vcopy)(void *in);
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
 * returns htable_t pointer to created hash table
 */
htable_t *htable_create(htable_hash hfunc, htable_keq keq, htable_cbs *cbs);
/* 
 * Destroys hash table
 * htable_t *ht: htable_t pointer to hash table to destroy
 */
void htable_destroy(htable_t *ht);

/*
 * Insert key-value pair into hashtable
 * htable_t *ht: hashtable to be inserted into
 * void *key: key
 * void *val: value
 */
void htable_insert(htable_t *ht, void *key, void *val);
/*
 * Removes entry from hashtable
 * htable_t *ht: hashtable to remove from
 * void *key: key of entry to remove
 */
void htable_remove(htable_t *ht, void *key);

/*
 * Gets value associated with given key 
 * htable_t *ht: hashtable to retrieve from
 * void *key: key
 * void **val: return pointer to value associated with key
 * returns 0 if key not in hashtable, 1 if key is
 */
int htable_get(htable_t *ht, void *key, void **val);

// Methods for enumerating through key-value pairs in hashtable
htable_enum_t *htable_enum_create(htable_t *ht);
int htable_enum_next(htable_enum_t *he, void **key, void **val);
void htable_enum_destroy(htable_enum_t *he);

#endif /*__HTABLE.H__*/
