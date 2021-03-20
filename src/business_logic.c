#include <stdbool.h>
#include <stdlib.h>
#include <sys/types.h>

#include "common.h"
#include "business_logic.h"
#include "database.h"

int reserve_space (uid_t user_id, uint64_t reservation_size){
	Record  record;
	int	return_value;

	return_value = get_user_record(user_id, &record);
	if (return_value == 1)
		return 0;
	else if (return_value == 3) {
		if (add_user_record(user_id, reservation_size, DEFAULT_BYTE_QUOTA, 0, DEFAULT_FILE_QUOTA) != 0)
			return 0;
		if (get_user_record(user_id, &record) != 0)
			return 0;
	}

	if ((record.byte_total + reservation_size) > record.byte_quota)
		return 0;
	else {
		if(change_user_record(user_id, reservation_size, 0, 0, 0) != 0)
			return 0;
	}
	return 1;
}

int update_reservation(uid_t user_id, uint64_t reservation_size, uint64_t num_used){
	int return_value;
	
	return_value = change_user_record(user_id, num_used-reservation_size, 0, 0, 0);
	if (return_value == 1)
		return 1;
	else if (return_value == 3) {
		if (add_user_record(user_id, reservation_size, DEFAULT_BYTE_QUOTA, 0, DEFAULT_FILE_QUOTA) != 0)
			return 1;
	}
	return 0;
}

int db_init()
{
	return database_init();
}