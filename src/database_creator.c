#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include "common.h"
#include "database_interface.h"

#define BLOCK_SIZE 63

int main(int argc, char **argv){
	FILE *fp;
	fp = fopen("database.csv", "w");
	if (fp == NULL)
		return 1;

	fprintf(fp, "%-20.20s,%20.20i,%20.20i\n", "user1", 0, 100);
	fprintf(fp, "%-20.20s,%20.20i,%20.20i\n", "user2", 50, 1000000);	
	fprintf(fp, "%-20.20s,%20.20i,%20.20i\n", "user3", 0, 200);

	fclose(fp);	

	Record record;
	char   *user = "user2";
	
	get_user_record(user, &record);
	printf("\nuser:%s total:%lu quota:%lu\n\n", record.user, record.total, record.quota);	
	
	change_user_record(user, 50, 0);
	get_user_record(user, &record);
	printf("\nuser:%s total:%lu quota:%lu\n\n", record.user, record.total, record.quota);	
	
	user = "user4";
	add_record(user, 2000, 100000);
	get_user_record(user, &record);
	printf("\nuser:%s total:%lu quota:%lu\n\n", record.user, record.total, record.quota);	
}
