#include "common.h"
#include "business_logic.h"
#include "database.h"

int reserve_space (uint64_t user_id, int reservation_size){
	
	if(!contains_user(user_id)){
		add_user(user_id);
	}

	int re = add_usage_record(user_id, reservation_size);
	return re;
}

int update_reservation (uint64_t user_id, int reservation_size, int num_used){
	int re = add_usage_record(user_id, num_used - reservation_size);
	return re;
}