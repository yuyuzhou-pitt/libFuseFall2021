#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include "common.h"
#include "database.h"
#include "business_logic.h"

#define BLOCK_SIZE 63

int main(int argc, char **argv){

	//create_empty_database();

	FILE *fp;
	fp = fopen("database.csv", "w+");
	if (fp == NULL)
		return 1;

	fprintf(fp, "%20.20i,%20.20i,%20.20i,%20.20i,%20.20i\n", 123456789, 0, 100, 0, 0);
	fprintf(fp, "%20.20i,%20.20i,%20.20i,%20.20i,%20.20i\n", 88888, 0, 1000000, 0, 0);	
	fprintf(fp, "%20.20i,%20.20i,%20.20i,%20.20i,%20.20i\n", 2478952, 0, 200, 0, 0);

	fclose(fp);	

	/*	
	Record record;
	uid_t user = 123456789;	

	get_user_record(user, &record);
	printf("user:%i total:%lu quota:%lu\n\n", record.user_id, record.byte_total, record.byte_quota);	
	
	change_user_record(user, 50, 0, 0, 0);
	get_user_record(user, &record);
	printf("user:%i total:%lu quota:%lu\n\n", record.user_id, record.byte_total, record.byte_quota);	
	
	user = 654123;
	add_user_record(user, 2000, 100000, 0, 0);
	get_user_record(user, &record);
	printf("user:%i total:%lu quota:%lu\n\n", record.user_id, record.byte_total, record.byte_quota);	

	print_all_records();
	
	Record record;

	database_init();
	
	reserve_space(123456789, 400);
	get_user_record(123456789, &record);
	printf("user:%i total:%lu quota:%lu\n\n", record.user_id, record.byte_total, record.byte_quota);	
	update_user_total(123456789,200);
	get_user_record(123456789, &record);
	printf("user:%i total:%lu quota:%lu\n\n", record.user_id, record.byte_total, record.byte_quota);	
	*/
}
