#include "common.h"
#include "database.h"

#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h> 

#include <limits.h> 
#include <fuse.h> 
#include <stdarg.h>
#include <string.h>

#include <stdint.h>

void
fullpath2 (const char *path, char *buf)
{
  char *basedir = (char *) fuse_get_context ()->private_data;

  strcpy (buf, basedir);
  strcat (buf, path);
}

void
log_data2(const char * format, ...){
    
  char fpath[1000];
  const char* lpath = "/../log.txt";
  fullpath2 (lpath, fpath);
  FILE * fp = fopen(fpath, "a");    

  va_list args;
  va_start(args, format);
  vfprintf(fp, format, args);
  va_end(args);

  fclose(fp);
}

// The database we do all the work on.
sqlite3 *db;

/*
 * Returns a user record
 * returns 1 if successful, 0 if unsuccessful 
 */
int contains_user(uint64_t user_id){
    sqlite3_stmt *stmt;
    int re = sqlite3_prepare_v2(db, "SELECT user_id FROM USER_QUOTAS WHERE user_id == ?", -1, &stmt, 0);    
    if (re != SQLITE_OK) {
        fprintf(stderr, "Failed to fetch data: %s\n", sqlite3_errmsg(db));
        return 1;
    }    
    
    sqlite3_bind_int64(stmt, 1, user_id);
    re = sqlite3_step(stmt);    
    sqlite3_finalize(stmt);

    return re == SQLITE_ROW;
}

/*
 * Changes the usage total for given user
 * uint64_t user_id
 * int32_t total_change: total number of bytes to add to usage
 * returns 1 if successful, 0 if unsuccessful
 */
int add_usage_record(uint64_t user_id, int32_t total_change){
    log_data2("ADDING USAGE\n");
    sqlite3_stmt *stmt;
    int re = sqlite3_prepare_v2(db, "UPDATE USER_QUOTAS SET usage_total = usage_total + ? WHERE user_id = ?", -1, &stmt, 0);    
    if (re != SQLITE_OK) {
        log_data2("Failed to fetch data (%d): %s\n", re, sqlite3_errmsg(db));
        return 1;
    }    
    
    sqlite3_bind_int64(stmt, 1, total_change);
    sqlite3_bind_int64(stmt, 2, user_id);
    re = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (re != SQLITE_DONE) {
        log_data2("Failed to fetch data (%d): %s\n", re, sqlite3_errmsg(db));
    }

    print_all();
    return re == SQLITE_DONE;
}

/*
 * Changes the file total for given user
 * uint64_t user_id
 * int32_t total_change: total number of files to add to the record
 * returns 1 if successful, 0 if unsuccessful
 */
int add_file_record(uint64_t user_id, int32_t total_change){
    sqlite3_stmt *stmt;
    int re = sqlite3_prepare_v2(db, "UPDATE USER_QUOTAS SET file_total = file_total + ? WHERE user_id = ?", -1, &stmt, 0);    
    if (re != SQLITE_OK) {
        fprintf(stderr, "Failed to fetch data (%d): %s\n", re, sqlite3_errmsg(db));
        return 1;
    }    
    
    sqlite3_bind_int64(stmt, 1, total_change);
    sqlite3_bind_int64(stmt, 2, user_id);
    re = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (re != SQLITE_DONE) {
        fprintf(stderr, "Failed to fetch data (%d): %s\n", re, sqlite3_errmsg(db));
    }
    return re == SQLITE_DONE;
}

void clear_db(){
    const char *sql_stmt = "DELETE FROM USER_QUOTAS";
    char *error_message;
    sqlite3_stmt *stmt;

    int re = sqlite3_exec(db, sql_stmt, NULL, 0, &error_message);
    if(re !=  SQLITE_OK){
        printf("Error: %s\n",error_message);
    }
}

int print_row(void *unused, int argc, char **argv, char **col_name) {    
    (void*)unused;
    
    for (int i = 0; i < argc; i++) {
        log_data2("%2s |\t", argv[i]);
    }
    log_data2("\n");
    
    return 0;
}

void 
print_all(){
    log_data2("Printing everything.\n");
    const char *sql_stmt = "SELECT * FROM USER_QUOTAS";
    char *error_message;
    sqlite3_stmt *stmt;

    int re = sqlite3_exec(db, sql_stmt, print_row, 0, &error_message);
    if(re !=  SQLITE_OK){
        log_data2("Error: %s\n",error_message);
        return;
    }
}

/*
 * Adds a record to the database
 * const char *user: string name of user
 * int64_t total: total bytes used by user
 * int64_t quota: quota of user
 * returns 0 if successful, corresponsing SQLite error code if unsuccessful
 */
int add_user(uint64_t user_id){
    fprintf(stderr, "Adding user %lu\n", user_id);
    const char *sql_stmt = "INSERT INTO USER_QUOTAS(user_id, usage_quota, file_quota) VALUES (?, ?, ?)";
    sqlite3_stmt *stmt;
    int re = sqlite3_prepare_v2(db, sql_stmt, -1, &stmt, NULL);
    if(re !=  SQLITE_OK){
        return re;
    }

    sqlite3_bind_int64(stmt, 1, user_id);
    sqlite3_bind_int64(stmt, 2, DEFAULT_USAGE_QUOTA);
    sqlite3_bind_int64(stmt, 3, DEFAULT_FILE_QUOTA);

    re = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if(re != SQLITE_DONE){
        return re;
    }

    return 0;
}

int
db_init(const char* path){
   int re = sqlite3_open_v2(path, &db, SQLITE_OPEN_CREATE | SQLITE_OPEN_READWRITE, NULL);
   if(re) {
      log_data2("Can't open database (%d): %s\n", re, sqlite3_errmsg(db));
      return 0;
   } 

    char *sql = 
    "CREATE TABLE IF NOT EXISTS USER_QUOTAS ("
        "user_id INTEGER NOT NULL,"
        "usage_quota INTEGER DEFAULT DEFAULT_USAGE_QUOTA NOT NULL,"
        "usage_total INTEGER DEFAULT 0 NOT NULL,"
        "file_quota INTEGER DEFAULT DEFAULT_FILE_QUOTA NOT NULL,"
        "file_total INTEGER DEFAULT 0 NOT NULL,"

        "constraint USER_QUOTAS_PK PRIMARY KEY (user_id),"
        "constraint pos_usage_quota check(usage_quota >= 0),"
        "constraint pos_usage_total check(usage_total >= 0),"
        "constraint pos_file_quota  check(file_quota >= 0),"
        "constraint pos_file_total  check(file_total >= 0),"
        "constraint usage_quota_max check(usage_quota >= usage_total),"
        "constraint file_quota_max check(file_quota >= file_total)"
    ");";

    char *error_message;
    re = sqlite3_exec(db, sql, 0, 0, &error_message);
    if(re != SQLITE_OK){
      log_data2("Can't inialize database (%d): %s\n", re, sqlite3_errmsg(db));
      return 0;
    }

    log_data2("Initalized\n");
    return 1;
}

int
db_close(){
    int re = sqlite3_close(db);
    db = NULL; 
    log_data2("Closed\n");
    return re;
}

// int main(int argc, char* argv[]) {
//     db_init("test.db");
//     // Some tests to make sure the database works
//     fprintf(stderr, "Opened database successfully\n");
//     add_user(1);
//     add_user(4);
//     print_all();
//     add_usage_record(1, 5);
//     add_usage_record(1, 1000000000); // This should fail
//     print_all();

//     printf("C: %s \n", contains_user(0)? "true" : "false");
//     printf("C: %s \n", contains_user(1)? "true" : "false");
//     printf("C: %s \n", contains_user(2)? "true" : "false");
//     printf("C: %s \n", contains_user(3)? "true" : "false");
//     printf("C: %s \n", contains_user(4)? "true" : "false");
//     clear_db();
//     print_all();
//     db_close();
// }