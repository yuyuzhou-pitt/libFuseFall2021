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

void test_reserve_space(void)
{
	log_data_ExpectAnyArgs();
	log_data_ExpectAnyArgs();   //This method is expected to be called exactly twice
	get_user_record_ExpectAnyArgsAndReturn(1); //This method is expected to be called once and return 1
	TEST_ASSERT_FALSE(reserve_space(0,0)); //By manipulating return values we got reserve_space to return 0 or false
}
