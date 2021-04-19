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

#include "mock_mockable_fuse.h"
#include "ntapfuse_ops.h"

#include "mock_business_logic.h"
#include "mock_common.h"
#include "mock_user_locks.h"

struct fuse_context *fs ;
struct fuse_file_info *fi;

void setUp(void)
{
    fs = malloc(sizeof(struct fuse_context));
    fs -> private_data = "fuse/";
    fi = calloc(1, sizeof(struct fuse_context));
    fuse_get_context_ExpectAndReturn(fs);
    log_data_Ignore();
    print_all_Ignore();
}

void tearDown(void)
{
    free(fs);
    free(fi);
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

void test_rmdir(void){
    lock_user_mutex_ExpectAnyArgsAndReturn(0);
    unlock_user_mutex_ExpectAnyArgsAndReturn(0);

    rmdir_ExpectAnyArgsAndReturn(0);

    update_file_record_ExpectAnyArgsAndReturn(1);

    TEST_ASSERT_EQUAL_INT(0, ntapfuse_rmdir("path"));
}

void test_rmdir_fail(void){
    rmdir_ExpectAnyArgsAndReturn(1);

    TEST_ASSERT_TRUE(ntapfuse_rmdir("path"));
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

void test_mknod_fail_update(void){
    lock_user_mutex_ExpectAnyArgsAndReturn(0);
    unlock_user_mutex_ExpectAnyArgsAndReturn(0);

    update_file_record_ExpectAnyArgsAndReturn(0);

    TEST_ASSERT_TRUE(ntapfuse_mknod("path", 1, 2));
}

void test_mkdir(void){
    lock_user_mutex_ExpectAnyArgsAndReturn(0);
    unlock_user_mutex_ExpectAnyArgsAndReturn(0);

    update_file_record_ExpectAnyArgsAndReturn(1);
    mkdir_ExpectAnyArgsAndReturn(0);

    TEST_ASSERT_EQUAL_INT(0, ntapfuse_mkdir("path", 1));
}

void test_mkdir_fail(void){
    lock_user_mutex_ExpectAnyArgsAndReturn(0);
    unlock_user_mutex_ExpectAnyArgsAndReturn(0);

    update_file_record_ExpectAnyArgsAndReturn(1);
    mkdir_ExpectAnyArgsAndReturn(-1);
    update_file_record_ExpectAnyArgsAndReturn(1);

    TEST_ASSERT_TRUE(ntapfuse_mkdir("path", 1));
}

void test_mkdir_fail_update(void){
    lock_user_mutex_ExpectAnyArgsAndReturn(0);
    unlock_user_mutex_ExpectAnyArgsAndReturn(0);

    update_file_record_ExpectAnyArgsAndReturn(0);

    TEST_ASSERT_TRUE(ntapfuse_mkdir("path", 1));
}


void test_write(void){
    int size = 100;
    int off = 2;
    lock_user_mutex_ExpectAnyArgsAndReturn(0);
    unlock_user_mutex_ExpectAnyArgsAndReturn(0);

    update_usage_record_ExpectAnyArgsAndReturn(1);
    // pwrite_ExpectAnyArgsAndReturn(size);
    update_usage_record_ExpectAnyArgsAndReturn(1);

    ntapfuse_write("path", NULL, size, off, fi);
}

void test_write_fail_update(void){
    int size = 100;
    int off = 2;
    lock_user_mutex_ExpectAnyArgsAndReturn(0);
    unlock_user_mutex_ExpectAnyArgsAndReturn(0);

    update_usage_record_ExpectAnyArgsAndReturn(0);

    ntapfuse_write("path", NULL, size, off, fi);
}

void test_link(void){
    fuse_get_context_ExpectAndReturn(fs);
    lock_user_mutex_ExpectAnyArgsAndReturn(0);
    unlock_user_mutex_ExpectAnyArgsAndReturn(0);

    update_file_record_ExpectAnyArgsAndReturn(1);
    update_usage_record_ExpectAnyArgsAndReturn(1);
    link_ExpectAnyArgsAndReturn(0);

    TEST_ASSERT_EQUAL_INT(0, ntapfuse_link("path", "dest"));
}

void test_link_fail_link(void){
    fuse_get_context_ExpectAndReturn(fs);
    lock_user_mutex_ExpectAnyArgsAndReturn(0);
    unlock_user_mutex_ExpectAnyArgsAndReturn(0);

    update_file_record_ExpectAnyArgsAndReturn(1);
    update_usage_record_ExpectAnyArgsAndReturn(1);
    link_ExpectAnyArgsAndReturn(-1);
    update_usage_record_ExpectAnyArgsAndReturn(1);
    update_file_record_ExpectAnyArgsAndReturn(1);

    TEST_ASSERT_TRUE(ntapfuse_link("path", "dest"));
}

void test_link_fail_update_file(void){
    fuse_get_context_ExpectAndReturn(fs);
    lock_user_mutex_ExpectAnyArgsAndReturn(0);
    unlock_user_mutex_ExpectAnyArgsAndReturn(0);

    update_file_record_ExpectAnyArgsAndReturn(0);

    TEST_ASSERT_TRUE(ntapfuse_link("path", "dest"));
}

void test_link_fail_update_usage(void){
    fuse_get_context_ExpectAndReturn(fs);
    lock_user_mutex_ExpectAnyArgsAndReturn(0);
    unlock_user_mutex_ExpectAnyArgsAndReturn(0);

    update_file_record_ExpectAnyArgsAndReturn(1);
    update_usage_record_ExpectAnyArgsAndReturn(0);
    update_file_record_ExpectAnyArgsAndReturn(1);

    TEST_ASSERT_TRUE(ntapfuse_link("path", "dest"));
}

void test_chown(void){
    int uid = 10;
    int gid = 20;

    lock_user_mutex_ExpectAnyArgsAndReturn(0);
    unlock_user_mutex_ExpectAnyArgsAndReturn(0);

    update_file_record_ExpectAnyArgsAndReturn(1);
    update_usage_record_ExpectAnyArgsAndReturn(1);
    chown_ExpectAnyArgsAndReturn(0);

    TEST_ASSERT_EQUAL_INT(0, ntapfuse_chown("path", uid, gid));
}

void test_chown_fail_chown(void){
    int uid = 10;
    int gid = 20;

    lock_user_mutex_ExpectAnyArgsAndReturn(0);
    unlock_user_mutex_ExpectAnyArgsAndReturn(0);

    update_file_record_ExpectAnyArgsAndReturn(1);
    update_usage_record_ExpectAnyArgsAndReturn(1);
    chown_ExpectAnyArgsAndReturn(-1);
    update_usage_record_ExpectAnyArgsAndReturn(1);
    update_file_record_ExpectAnyArgsAndReturn(1);

    TEST_ASSERT_TRUE(ntapfuse_chown("path", uid, gid));
}

void test_chown_fail_update_file(void){
    int uid = 10;
    int gid = 20;

    lock_user_mutex_ExpectAnyArgsAndReturn(0);
    unlock_user_mutex_ExpectAnyArgsAndReturn(0);

    update_file_record_ExpectAnyArgsAndReturn(0);

    TEST_ASSERT_TRUE(ntapfuse_chown("path", uid, gid));
}

void test_chown_fail_update_usage(void){
    int uid = 10;
    int gid = 20;

    lock_user_mutex_ExpectAnyArgsAndReturn(0);
    unlock_user_mutex_ExpectAnyArgsAndReturn(0);

    update_file_record_ExpectAnyArgsAndReturn(1);
    update_usage_record_ExpectAnyArgsAndReturn(0);
    update_file_record_ExpectAnyArgsAndReturn(1);

    TEST_ASSERT_TRUE(ntapfuse_chown("path", uid, gid));
}

/* Removed due to problems with src/mockable_fuse.h */

// void test_truncate(void){
//     int off = 10;

//     lock_user_mutex_ExpectAnyArgsAndReturn(0);
//     unlock_user_mutex_ExpectAnyArgsAndReturn(0);

//     update_usage_record_ExpectAnyArgsAndReturn(1);
//     truncate_ExpectAnyArgsAndReturn(-1);

//     //IDEK
//     update_usage_record_ExpectAnyArgsAndReturn(1);

//     //TEST_ASSERT_EQUAL_INT(0, ntapfuse_truncate("path", off));
//     ntapfuse_truncate("path", off);
// }

// void test_truncate_fail_truncate(void){
//     int off = 10;

//     lock_user_mutex_ExpectAnyArgsAndReturn(0);
//     unlock_user_mutex_ExpectAnyArgsAndReturn(0);

//     update_usage_record_ExpectAnyArgsAndReturn(1);
//     truncate_ExpectAnyArgsAndReturn(-1);
//     update_usage_record_ExpectAnyArgsAndReturn(1);

//     TEST_ASSERT_TRUE(ntapfuse_truncate("path", off));
// }

void test_truncate_fail_update(void){
    int off = 10;

    lock_user_mutex_ExpectAnyArgsAndReturn(0);
    unlock_user_mutex_ExpectAnyArgsAndReturn(0);

    update_usage_record_ExpectAnyArgsAndReturn(0);

    TEST_ASSERT_TRUE(ntapfuse_truncate("path", off));
}