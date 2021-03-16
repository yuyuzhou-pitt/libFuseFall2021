#include "common.h"
#include "database.h"

#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h> 

#include <limits.h> 
#include <fuse.h> 
#include <string.h>

#include <stdint.h>

// The database we do all the work on.
sqlite3 *db;

// See documentation in database.h
int contains_user(uint64_t user_id){
    sqlite3_stmt *stmt;
    int re = sqlite3_prepare_v2(db, "SELECT user_id FROM USER_QUOTAS WHERE user_id == ?", -1, &stmt, 0);    
    if (re != SQLITE_OK) {
        log_data("Failed to fetch data: %s\n", sqlite3_errmsg(db));
        return 1;
    }    
    
    sqlite3_bind_int64(stmt, 1, user_id);
    re = sqlite3_step(stmt);    
    sqlite3_finalize(stmt);

    return re == SQLITE_ROW;
}

// See documentation in database.h
int add_usage_record(uint64_t user_id, int64_t total_change){
    sqlite3_stmt *stmt;
    int re = sqlite3_prepare_v2(db, "UPDATE USER_QUOTAS SET usage_total = usage_total + ? WHERE user_id = ?", -1, &stmt, 0);    
    if (re != SQLITE_OK) {
        log_data("Failed to fetch data (%d): %s\n", re, sqlite3_errmsg(db));
        return 1;
    }    
    
    sqlite3_bind_int64(stmt, 1, total_change);
    sqlite3_bind_int64(stmt, 2, user_id);
    re = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (re != SQLITE_DONE) {
        log_data("Failed to fetch data (%d): %s\n", re, sqlite3_errmsg(db));
    }

    print_all();
    return re == SQLITE_DONE;
}

// See documentation in database.h
int add_file_record(uint64_t user_id, int64_t total_change){
    sqlite3_stmt *stmt;
    int re = sqlite3_prepare_v2(db, "UPDATE USER_QUOTAS SET file_total = file_total + ? WHERE user_id = ?", -1, &stmt, 0);    
    if (re != SQLITE_OK) {
        log_data("Failed to fetch data (%d): %s\n", re, sqlite3_errmsg(db));
        return 1;
    }    
    
    sqlite3_bind_int64(stmt, 1, total_change);
    sqlite3_bind_int64(stmt, 2, user_id);
    re = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (re != SQLITE_DONE) {
        log_data("Failed to fetch data (%d): %s\n", re, sqlite3_errmsg(db));
    }
    return re == SQLITE_DONE;
}

// See documentation in database.h
void clear_db(){
    const char *sql_stmt = "DELETE FROM USER_QUOTAS";
    char *error_message;
    sqlite3_stmt *stmt;

    int re = sqlite3_exec(db, sql_stmt, NULL, 0, &error_message);
    if(re !=  SQLITE_OK){
        log_data("Error: %s\n",error_message);
    }
}

//Helper function for print_all()
int print_row(void *unused, int argc, char **argv, char **col_name) {    
    (void*)unused;
    
    for (int i = 0; i < argc; i++) {
        log_data("%s:%4s\t|  ", col_name[i], argv[i]);
    }
    log_data("\n");
    
    return 0;
}

// See documentation in database.h
void print_all(){
    log_data("Printing everything in table:\n");
    const char *sql_stmt = "SELECT * FROM USER_QUOTAS";
    char *error_message;
    sqlite3_stmt *stmt;

    int re = sqlite3_exec(db, sql_stmt, print_row, 0, &error_message);
    if(re !=  SQLITE_OK){
        log_data("Error: %s\n",error_message);
        return;
    }
}

// See documentation in database.h
int add_user(uint64_t user_id){
    log_data("Adding user %lu into table\n", user_id);
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

// See documentation in database.h
int db_init(const char* path){
   int re = sqlite3_open_v2(path, &db, SQLITE_OPEN_CREATE | SQLITE_OPEN_READWRITE, NULL);
   if(re) {
      log_data("Can't open database (%d): %s\n", re, sqlite3_errmsg(db));
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
      log_data("Can't inialize database (%d): %s\n", re, sqlite3_errmsg(db));
      return 0;
    }

    log_data("Initalized\n");
    return 1;
}

// See documentation in database.h
int db_close(){
    int re = sqlite3_close(db);
    db = NULL; 
    log_data("Closed\n");
    return re;
}