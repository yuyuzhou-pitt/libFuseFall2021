#include "common.h"
#include "business_logic.h"
#include "database.h"

int update_usage_record(uid_t user_id, int64_t total_change){
	log_data("Reserving %lu bytes for user %i\n", total_change, user_id);
	int return_value = change_user_record(user_id, total_change, 0, 0, 0);
	if (return_value == DB_USER_NOT_EXIST) {
		if (add_user_record(user_id, total_change, DEFAULT_BYTE_QUOTA, 0, DEFAULT_FILE_QUOTA) != 0){
			return 0;
		}
	} else if (return_value != DB_SUCCESS){
		return 0;
	}

	return 1;
}

int update_file_record(uid_t user_id, int64_t total_change){
	log_data("Reserving %lu inodes for user %i\n", total_change, user_id);
	int return_value = change_user_record(user_id, 0, 0, total_change, 0);
	if (return_value == DB_USER_NOT_EXIST) {
		if (add_user_record(user_id, 0, DEFAULT_BYTE_QUOTA, total_change, DEFAULT_FILE_QUOTA) != 0){
			return 0;
		}
	} else if (return_value != DB_SUCCESS){
		return 0;
	}

	return 1;
}

void print_all(){
	print_all_records();
}

uid_t * db_init()
{
	log_data("Initializing database\n");
	return database_init();
}

int db_close()
{
	log_data("Closing database\n");
	return database_close();
}
