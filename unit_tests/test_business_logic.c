#include <sys/types.h>

#include "unity.h"
#include "business_logic.h"
#include "mock_common.h"
#include "mock_database.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void test_update_usage_dberror(void)
{
	log_data_ExpectAnyArgs();   //This method is expected to be called exactly twice
	change_user_record_ExpectAndReturn(1, 2, 0, 0, 0, DB_ERROR); 
	TEST_ASSERT_FALSE(update_usage_record(1,2)); //By manipulating return values we got reserve_space to return 0 or false
}

void test_update_file_dberror(void)
{
	log_data_ExpectAnyArgs();   //This method is expected to be called exactly twice
	change_user_record_ExpectAndReturn(1, 0, 0, 2, 0, DB_ERROR); 
	TEST_ASSERT_FALSE(update_file_record(1,2)); //By manipulating return values we got reserve_space to return 0 or false
}

void test_update_usage_user_not_exist_sucessfull_add(void)
{
	log_data_ExpectAnyArgs();   //This method is expected to be called exactly twice
	change_user_record_ExpectAndReturn(1, 2, 0, 0, 0, DB_USER_NOT_EXIST); 
	add_user_record_ExpectAndReturn(1, 2, DEFAULT_BYTE_QUOTA, 0, DEFAULT_FILE_QUOTA, DB_SUCCESS);
	TEST_ASSERT_TRUE(update_usage_record(1,2)); //By manipulating return values we got reserve_space to return 0 or false
}

void test_update_file_user_not_exist_sucessfull_add(void)
{
	log_data_ExpectAnyArgs();   //This method is expected to be called exactly twice
	change_user_record_ExpectAndReturn(1, 0, 0, 2, 0, DB_USER_NOT_EXIST); 
	add_user_record_ExpectAndReturn(1, 0, DEFAULT_BYTE_QUOTA, 2, DEFAULT_FILE_QUOTA, DB_SUCCESS);   
	TEST_ASSERT_TRUE(update_file_record(1,2)); //By manipulating return values we got reserve_space to return 0 or false
}

void test_update_usage_user_not_exist_bad_add(void)
{
	log_data_ExpectAnyArgs();   //This method is expected to be called exactly twice
	change_user_record_ExpectAndReturn(1, 2, 0, 0, 0, DB_USER_NOT_EXIST); 
	add_user_record_ExpectAndReturn(1, 2, DEFAULT_BYTE_QUOTA, 0, DEFAULT_FILE_QUOTA, DB_ERROR);
	TEST_ASSERT_FALSE(update_usage_record(1,2)); //By manipulating return values we got reserve_space to return 0 or false
}

void test_update_file_user_not_exist_bad_add(void)
{
	log_data_ExpectAnyArgs();   //This method is expected to be called exactly twice
	change_user_record_ExpectAndReturn(1, 0, 0, 2, 0, DB_USER_NOT_EXIST); 
	add_user_record_ExpectAndReturn(1, 0, DEFAULT_BYTE_QUOTA, 2, DEFAULT_FILE_QUOTA, DB_ERROR);   
	TEST_ASSERT_FALSE(update_file_record(1,2)); //By manipulating return values we got reserve_space to return 0 or false
}

void test_print_all(void)
{
	print_all_records_ExpectAndReturn(DB_SUCCESS);
	print_all();
}

void test_db_init(void)
{
	log_data_ExpectAnyArgs();   
	database_init_ExpectAndReturn(DB_SUCCESS);
	db_init();
}

void test_db_close(void)
{
	log_data_ExpectAnyArgs();   
	database_close_ExpectAndReturn(DB_SUCCESS);
	db_close();
}
