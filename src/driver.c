#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "htable_data_cache.h"
#include "common.h"

int main(int argc, char **argv)
{
	htable_data_cache 	*h;
	htable_data_cache_enum  *he;

	Record *record = malloc(sizeof(Record));
	Record *recordTwo = malloc(sizeof(Record));
	strncpy(record->user, "user1", 21);
	record->total = 50;
	record->quota = 100;
	
	strncpy(recordTwo->user, "user2", 21);
	recordTwo->total = 0;
	recordTwo->quota = 20000;

	Record *recordThree;	

	h = htable_data_cache_create();
	
	htable_data_cache_insert(h, (const char *)&(record->user), record);
	htable_data_cache_get(h, "user1", &recordThree);
	printf("User:%s Total:%lu Quota:%lu\n", recordThree->user, recordThree->total, recordThree->quota);

	htable_data_cache_insert(h, (const char *)&(recordTwo->user), recordTwo);
	htable_data_cache_get(h, "user2", &recordThree);
	printf("User:%s Total:%lu Quota:%lu\n", recordThree->user, recordThree->total, recordThree->quota);

	htable_data_cache_remove(h, "user2");

	const char *key;
	he = htable_data_cache_enum_create(h);
	while (htable_data_cache_enum_next(he, &key, &recordThree)) {
		printf("key:%s ", key);
		printf("User:%s Total:%lu Quota:%lu\n", recordThree->user, recordThree->total, recordThree->quota);
	}
	
	htable_data_cache_enum_destroy(he);
	htable_data_cache_destroy(h);
}
