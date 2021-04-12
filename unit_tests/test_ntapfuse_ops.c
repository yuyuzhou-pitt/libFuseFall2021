// #include <stdio.h>
// #include <stdarg.h>

// #include <errno.h>
// // #include <dirent.h>
// #include <limits.h>
// #include <string.h>

//#include <sys/xattr.h>
// #include <sys/types.h>

#include <sys/stat.h>
#include <stdlib.h>

#include "ntapfuse_ops.h"

#include "mock_mockable_fuse.h"
#include "mock_business_logic.h"
#include "mock_common.h"
#include "mock_user_locks.h"

struct fuse_context *fs ;

void setUp(void)
{
    fs = malloc(sizeof(struct fuse_context));
    fs -> private_data = "fuse/";
    fuse_get_context_ExpectAndReturn(fs);
    log_data_Ignore();
    print_all_Ignore();
}

void tearDown(void)
{
    free(fs);
}

int custom_callback_update_usage_record(uid_t user_id, int64_t total_change, int num_of_call){
    TEST_ASSERT_TRUE(0);
    return 0;
}

void test_unlink(void){
    lock_user_mutex_ExpectAnyArgsAndReturn(0);
    unlock_user_mutex_ExpectAnyArgsAndReturn(0);

    unlink_ExpectAnyArgsAndReturn(0);

    update_file_record_ExpectAnyArgsAndReturn(1);
    update_usage_record_ExpectAnyArgsAndReturn(1);

    TEST_ASSERT_EQUAL_INT(0, ntapfuse_unlink("path"));
}

void test_unlink_fail(void){
    unlink_ExpectAnyArgsAndReturn(1);

    TEST_ASSERT_TRUE(ntapfuse_unlink("path"));
}

void test_mknod(void){
    lock_user_mutex_ExpectAnyArgsAndReturn(0);
    unlock_user_mutex_ExpectAnyArgsAndReturn(0);

    update_file_record_ExpectAnyArgsAndReturn(1);
    mknod_ExpectAndReturn("fuse/path", 1, 2, 0);

    TEST_ASSERT_EQUAL_INT(0, ntapfuse_mknod("path", 1, 2));
}

void test_mknod_fail(void){
    lock_user_mutex_ExpectAnyArgsAndReturn(0);
    unlock_user_mutex_ExpectAnyArgsAndReturn(0);

    update_file_record_ExpectAnyArgsAndReturn(1);
    mknod_ExpectAndReturn("fuse/path", 1, 2, -1);
    update_file_record_ExpectAnyArgsAndReturn(1);

    TEST_ASSERT_TRUE(ntapfuse_mknod("path", 1, 2));
}
