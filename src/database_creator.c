#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include "common.h"
#include "database.h"

#define BLOCK_SIZE 63

int main(int argc, char **argv){
	FILE *fp;
	fp = fopen("database.csv", "w");
	if (fp == NULL)
		return 1;

	fprintf(fp, "%20.20i,%20.20i,%20.20i\n", 123456789, 0, 100);
	fprintf(fp, "%20.20i,%20.20i,%20.20i\n", 88888, 50, 1000000);	
	fprintf(fp, "%20.20i,%20.20i,%20.20i\n", 2478952, 0, 200);

	fclose(fp);	

	Record record;
	uid_t user = 123456789;	

	get_user_record(user, &record);
	printf("user:%i total:%lu quota:%lu\n\n", record.user_id, record.total, record.quota);	
	
	change_user_record(user, 50, 0);
	get_user_record(user, &record);
	printf("user:%i total:%lu quota:%lu\n\n", record.user_id, record.total, record.quota);	
	
	user = 654123;
	add_user_record(user, 2000, 100000);
	get_user_record(user, &record);
	printf("user:%i total:%lu quota:%lu\n\n", record.user_id, record.total, record.quota);	

	print_all_records();
}
