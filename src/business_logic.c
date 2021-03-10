#include "common.h"
#include "business_logic.h"
#include "database_interface.h"

int check_action (const char *user, int size_change){
	Record record;
	
	if (get_user_record(user, &record) != 0)
		return 1;
	
	if ((record.total + size_change) <= record.quota)
		return 1;
	else 
		return 0;
}

int update_user_total (const char *user, int total_change){
	Record record;
	if(get_user_record(user, &record)){
		return add_record(user, total_change, DEFAULT_QUOTA);
	}
	else if(change_user_record(user, total_change, 0) == 0){
		return 0;
	}

}
