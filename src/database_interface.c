#include <stdint.h>
#include <stdio.h>

#include "common.h"
#include "database_interface.h"

#define BLOCK_SIZE 160

int get_user_record(const char *user, record **record)
{
	uint32_t block;
	char[BLOCK_SIZE} buffer;
	//find block
	if(fseek(fp, block * BLOCK_SIZE, SEEK_SET) != 0)
		return 1;
	fread(buffer, BLOCK_SIZE, fp);
	*record->user = strtok(buffer, ",");
	*record->total = strtok(buffer, ",");
	*record->quota = strtok(buffer, ",");
	return 0;
}

int change_user_record(const char *user, int32_t total_change, int32_t quota_change)
{
	record *record;
	File *fp;
	// get record
	record->total += total_change;
	record->quota += quota_change;

	// fseek to record block
	if(fseek(fp, record->block * BLOCK_SIZE, SEEK_SET) != 0)
		return 1;
	
	fprintf(fp, "%30.30s,%64.64lu,%64.64lu\n", record->user, record->total, record->quota);
	fclose(fp);
	return 0;
}

int add_record(const char *user, int64_t total, int64_t quota)
{
	File *fp;
	if(fp = fopen("database.csv", "a");
	if(fp == NULL)
		return 1;
	
	if(fprintf(fp, "%30.30s,%64.64lu,%64.64lu\n", user, total, quota) !=  0)
		return 1;
	
	fclose(fp);
	return 0;
}
