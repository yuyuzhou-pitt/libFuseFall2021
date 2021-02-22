#include <business_logic.h>
#include <database_interface.h>

int check_action (const char *user, int size){
	total = user_total(user);
	quota = user_quota(user);
	if ((total + size) <= quota)
		return 1;
	else 
		return 0;
}

int update_user_total (const char *user, int size){
	change_user_total(user, size)
	return 1;
}
