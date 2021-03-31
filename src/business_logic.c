#include "common.h"
#include "business_logic.h"
#include "database.h"

int ensure_user_exists(uid_t user_id){
	// if(!contains_user(user_id)){
	// 	return add_user(user_id);
	// }
	return 0;
}

<<<<<<< HEAD

int update_usage_record(uid_t user_id, int64_t total_change){
	// This was written by Greg, not exhaustively tested.
	int return_value = change_user_record(user_id, total_change, 0, 0, 0);
=======
	log_data("Reserving %lu bytes for user %i\n", reservation_size, user_id);
	return_value = get_user_record(user_id, &record);
>>>>>>> 24e7766083140360819ff5dbcb38f693f4353e16
	if (return_value == 3) {
		if (add_user_record(user_id, total_change, DEFAULT_BYTE_QUOTA, 0, DEFAULT_FILE_QUOTA) != 0){
			return 1;
		}
	} else if (return_value != 0){
		return 1;
	}
	return 0;
}

int update_file_record(uid_t user_id, int64_t total_change){
	// This was written by Greg, not exhaustively tested.
	int return_value = change_user_record(user_id, 0, 0, total_change, 0);
	if (return_value == 3) {
		if (add_user_record(user_id, 0, DEFAULT_BYTE_QUOTA, total_change, DEFAULT_FILE_QUOTA) != 0){
			return 1;
		}
	} else if (return_value != 0){
		return 1;
	}
	return 0;
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
