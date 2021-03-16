#include "common.h"
#include "business_logic.h"
#include "database.h"

int check_action(uid_t user_id, int size_change){
	Record *record;
	
	if (get_user_record(user_id, record) != 0)
		return 1;
	
	if ((record->total + size_change) <= record->quota)
		return 1;
	else 
		return 0;
}

int update_user_total(uid_t user_id, int total_change){
	return change_user_record(user_id, total_change, 0);
}
