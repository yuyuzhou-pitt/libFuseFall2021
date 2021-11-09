/**
 * Project: ntapfuse
 * Author: Samuel Kenney <samuel.kenney48@gmail.com>
 *         August Sodora III <augsod@gmail.com>
 * File: ntapfuse_ops.c
 * License: GPLv3
 *
 * ntapfuse is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ntapfuse is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ntapfuse. If not, see <http://www.gnu.org/licenses/>.
 */
#define _XOPEN_SOURCE 500

#include "ntapfuse_ops.h"
#include <stdio.h>

#include <stdlib.h>

#include <errno.h>
#include <dirent.h>
#include <limits.h>
#include <string.h>
#include <unistd.h>
//#include <mysql/mysql.h>

#include <sys/xattr.h>
#include <sys/types.h>
#include <sys/file.h>

//Added vfs.h to get the inode of a file
#include <sys/vfs.h>
//Include time.h to convert to local time
#include <time.h>




/**
 * Appends the path of the root filesystem to the given path, returning
 * the result in buf.
 */
void
fullpath (const char *path, char *buf)
{

  char *basedir = (char *) fuse_get_context ()->private_data;

  strcpy (buf, basedir);
  strcat (buf, path);
}

// Function to make log writes easier
void func_log(const char * message) {
  int write_all_functions = 1;

  if (!write_all_functions) {
	return;
  }

  // Code to write to a file whenever the function is called.
  char log_path[PATH_MAX];
  fullpath("/log", log_path);
  FILE * log = fopen(log_path, "a+");
  fputs(message, log);
  fclose(log);
}

// Use to log important writes? Might not need
void user_log(const char * message) {
  // Code to write to a file whenever the function is called.
  char log_path[PATH_MAX];
  fullpath("/log", log_path);
  FILE * log = fopen(log_path, "a+");
  fputs(message, log);
  fclose(log);
}

/*
helper function for db writes
takes the userID of the file owner, the change in file size in off_t
off_t is a signed variable, so a negative number will decrement log
CREDIT: Connor, taken from write implementation
*/
int
db_update(uid_t userID, off_t size)
{
//open database path, creates if nonexistant
  char db_path[PATH_MAX];
  fullpath("/db", db_path);
  FILE * lp = fopen(db_path, "a+");
  
//create temporary path for database ops
  //creates if nonexistant
  char temp_path[PATH_MAX];
  fullpath("/temp", temp_path);
  FILE * temp = fopen(temp_path, "w+");
  int used, max, uid;   // Variables passed by reference into sscanf
  char line[80];        // char * stores the current line being read
  int user_found = 0;   // Boolean for determining if we need to create a user
  
//iterates through database
  while (fgets(line, 80, lp) != NULL) {

	  // Make sure sscanf got the right number of items read
	  if (sscanf(line, "%d\t%d\t%d\n", &uid, &used, &max) != 3) { /* TODO: Error handling */ }

	  // Write line unchanged to temp file if it doesn't match
	  if (user_found || uid != userID) {
		  fputs(line, temp);
		  continue;
	  }
		
	  // Check and enforce space guidelines
	  if (used + size > max) { 
  		fclose(lp);        // Close orig file
  		fclose(temp);      // Close temp file
		remove(temp_path); // Delete temp file
		return -1;         // Return error TODO FIND CORRECT CODE FOR NOT ENOUGH SPACE
	  }

	  // Update size
	  used += size;

	  // Write updated data to temp file
	  fprintf(temp, "%d\t%d\t%d\n", uid, used, max);
	  user_found = 1;

	// Add database operation to log
	char log_path[PATH_MAX];
	fullpath("/log", log_path);
	FILE * log = fopen(log_path, "a+");
	fprintf(log, "User %d wrote %d bytes and now has %d \n", uid, size, used);
	fclose(log);

  }
    if (!user_found) {
	char log_path[PATH_MAX];
	fullpath("/log", log_path);
	FILE * log = fopen(log_path, "a+");

	fprintf(log, "Entry for user %d added to database", userID);

	  // Ensure user's first write will not be over max size
	  if (size > 4096) { // TODO CHANGE TO A DEFAULT QUOTA VARIABLE
		// Still add user, just set used to 0.
	  	fprintf(temp, "%d\t%d\t%d\n", userID, 0, 4096);

  		fclose(lp);        // Close orig file
  		fclose(temp);      // Close temp file
		remove(temp_path); // Delete temp file
		fclose(log);
		return -1;         // Return error TODO FIND CORRECT CODE FOR NOT ENOUGH SPACE
	  }
	  fprintf(temp, "%d\t%d\t%d\n", userID, size, 4096);

	fprintf(log, "User %d wrote %d bytes", userID, size);
	fclose(log);
  }
  // Close files
  fclose(lp);
  fclose(temp);
  
  //Swap files
  char swap_path[PATH_MAX];
  fullpath("/swap", swap_path); // We just need a path to swap, no file necessary here

  rename(db_path, swap_path); // log -> swap (keep ref to delete later)
  rename(temp_path, db_path); // temp -> log (what we wrote to is now the real log file)
  remove(swap_path);	       // delete swap (original log)

  return 0;
};

/* The following functions describe FUSE operations. Each operation appends
   the path of the root filesystem to the given path in order to give the
   mirrored path. */

int
ntapfuse_getattr (const char *path, struct stat *buf)
{
  func_log("getattr called\n");

  char fpath[PATH_MAX];
  fullpath (path, fpath);

  return lstat (fpath, buf) ? -errno : 0;
}

int
ntapfuse_readlink (const char *path, char *target, size_t size)
{
  func_log("readlink called\n");
  char fpath[PATH_MAX];
  fullpath (path, fpath);

  return readlink (fpath, target, size) < 0 ? -errno : 0;
}

int
ntapfuse_mknod (const char *path, mode_t mode, dev_t dev)
{
  func_log("mknod called\n");
  char fpath[PATH_MAX];
  fullpath (path, fpath);

  return mknod (fpath, mode, dev) ? -errno : 0;
}

int
ntapfuse_mkdir (const char *path, mode_t mode)
{
  func_log("mkdir called\n");
  char fpath[PATH_MAX];
  fullpath (path, fpath);

  return mkdir (fpath, mode | S_IFDIR) ? -errno : 0;
}

int
ntapfuse_unlink (const char *path)
{
  func_log("unlink called\n");
  char fpath[PATH_MAX];
  fullpath (path, fpath);

  return unlink (fpath) ? -errno : 0;
}

int
ntapfuse_rmdir (const char *path)
{
  func_log("rmdir called\n");
  char fpath[PATH_MAX];
  fullpath (path, fpath);

  return rmdir (fpath) ? -errno : 0;
}

int
ntapfuse_symlink (const char *path, const char *link)
{
  func_log("symlink called\n");
  char flink[PATH_MAX];
  fullpath (link, flink);

  return symlink (path, flink) ? -errno : 0;
}

int
ntapfuse_rename (const char *src, const char *dst)
{
  func_log("rename called\n");
  char fsrc[PATH_MAX];
  fullpath (src, fsrc);

  char fdst[PATH_MAX];
  fullpath (dst, fdst);

  return rename (fsrc, fdst) ? -errno : 0;
}

int
ntapfuse_link (const char *src, const char *dst)
{
  func_log("link called\n");
  char fsrc[PATH_MAX];
  fullpath (src, fsrc);

  char fdst[PATH_MAX];
  fullpath (dst, fdst);

  return link (fsrc, fdst) ? -errno : 0;
}

int
ntapfuse_chmod (const char *path, mode_t mode)
{
  func_log("chmod called\n");
  char fpath[PATH_MAX];
  fullpath (path, fpath);

  return chmod (fpath, mode) ? -errno : 0;
}

int
ntapfuse_chown (const char *path, uid_t uid, gid_t gid)
{
  func_log("chown called\n");
  char fpath[PATH_MAX];
  fullpath (path, fpath);

  return chown (fpath, uid, gid) ? -errno : 0;
}

//truncate reduces or expands a file to length # of bytes.
int
ntapfuse_truncate (const char *path, off_t length)
{
  func_log("truncate called\n");
  char fpath[PATH_MAX];
  fullpath (path, fpath);
  //open file information
  struct stat file_stat;
  stat(fpath,  &file_stat); // TODO ERROR CHECK
  
  off_t change;
  if(file_stat.st_size > length)change= length - file_stat.st_size;
  if(file_stat.st_size < length)change= file_stat.st_size -length;
  if(file_stat.st_size = length)change= 0;
  //callstat db helper method, fails on return
  if(db_update(file_stat.st_uid, change)!=0 ) return -1;

  return truncate (fpath, length) ? -errno : 0;
}

int
ntapfuse_utime (const char *path, struct utimbuf *buf)
{
  func_log("utime called\n");
  char fpath[PATH_MAX];
  fullpath (path, fpath);

  return utime (fpath, buf) ? -errno : 0;
}

int
ntapfuse_open (const char *path, struct fuse_file_info *fi)
{
  func_log("open called\n");
  char fpath[PATH_MAX];
  fullpath (path, fpath);

  int fh = open (fpath, fi->flags);
  if (fh < 0)
    return -errno;

  fi->fh = fh;

  return 0;
}

int
ntapfuse_read (const char *path, char *buf, size_t size, off_t off,
	   struct fuse_file_info *fi)
{
  func_log("read called\n");
  return pread (fi->fh, buf, size, off) < 0 ? -errno : size;
}

int
ntapfuse_write (const char *path, const char *buf, size_t size, off_t off,
	    struct fuse_file_info *fi)
{
  func_log("write called\n");
  // Get full path, starts out every function
  //char fpath[PATH_MAX];
  //fullpath (path, fpath);

  // ------------------------------------------------------------------
  // ------------------------------------------------------------------
  // STEP 1: OPEN LOG FILE AND CHECK IF USER HAS ENOUGH AVAILABLE SPACE
  // ------------------------------------------------------------------
  // ------------------------------------------------------------------

  char lock_path[PATH_MAX];
  fullpath("/lock", lock_path);

  int count = 1;
  int lock = 0;
  int lfd = 0;
  while (1) {

	  lfd = open(lock_path, O_RDWR | O_CREAT, 0666);
	  lock = flock(lfd, LOCK_EX | LOCK_NB);
	  if (lock == 0) {
		break;
	  }
	  sleep(1);
	  count += 1;
	  if (count > 10) {
		  return -1;
	  }
  }

  // Open db file with c standard library
  char db_path[PATH_MAX];
  fullpath("/db", db_path);
  FILE * lp = fopen(db_path, "a+");

  // Create a temp file to write to
  char temp_path[PATH_MAX];
  fullpath("/temp", temp_path);
  FILE * temp = fopen(temp_path, "w+");

//update user userID to get the file's owner
  uid_t userID = fuse_get_context()->uid;
  int uid, used, max;   // Variables that are passed by reference into sscanf
  char line[80];        // char * that stores the current line being read
  int user_found = 0;   // Boolean for determining if we need to create a user

  struct stat file_stat;
  fstat(fi->fh,  &file_stat); // TODO ERROR CHECK

  time_t raw_time = file_stat.st_mtime;
  struct tm * time_info;

  time(&raw_time);
  time_info = localtime(&raw_time);

  // Iterate through all lines in original db file
  while (fgets(line, 80, lp) != NULL) {

	  // Make sure sscanf got the right number of items read
	  if (sscanf(line, "%d\t%d\t%d\n", &uid, &used, &max) != 3) { /* TODO: Error handling */ }

	  // Write line unchanged to temp file if it doesn't match
	  if (user_found || uid != userID) {
		  fputs(line, temp);
		  continue;
	  }
		
	  // Check and enforce space guidelines
	  if (used + size > max) { 
  		close(fi->fh);     // Close file being written
  		fclose(lp);        // Close orig file
  		fclose(temp);      // Close temp file
		remove(temp_path); // Delete temp file
		return -1;         // Return error TODO FIND CORRECT CODE FOR NOT ENOUGH SPACE
	  }

	  // Update size
	  used += size;

	  // Write updated data to temp file
	  fprintf(temp, "%d\t%d\t%d\n", uid, used, max);
	  user_found = 1;

	// Add database operation to log
	char log_path[PATH_MAX];
	fullpath("/log", log_path);
	FILE * log = fopen(log_path, "a+");
	fprintf(log, "User %d wrote %d bytes - %s", userID, size, asctime(time_info));
	fclose(log);
  }

  // If the program goes through the whole file and doesn't find the user,
  // we need to add them to the file with a default setup.
  if (!user_found) {
	char log_path[PATH_MAX];
	fullpath("/log", log_path);
	FILE * log = fopen(log_path, "a+");

	fprintf(log, "Entry for user %d added to database - %s", userID, asctime(time_info));

	  // Ensure user's first write will not be over max size
	  if (size > 4096) { // TODO CHANGE TO A DEFAULT QUOTA VARIABLE
		// Still add user, just set used to 0.
	  	fprintf(temp, "%d\t%d\t%d\n", userID, 0, 4096);

  		close(fi->fh);     // Close file being written
  		fclose(lp);        // Close orig file
  		fclose(temp);      // Close temp file
		remove(temp_path); // Delete temp file
		fclose(log);
		return -1;         // Return error TODO FIND CORRECT CODE FOR NOT ENOUGH SPACE
	  }
	  fprintf(temp, "%d\t%d\t%d\n", userID, size, 4096);

	fprintf(log, "User %d wrote %d bytes - %s", userID, size, asctime(time_info));
	fclose(log);
  }
  // Close files
  fclose(lp);
  fclose(temp);


  // TODO: THIS SWAP STUFF SHOULD BE AFTER STEP 2 JUST IN CASE THE FILE
  // WRITE FAILS FOR SOME REASON
  // File write successful? Do swap and new file
  // File write unsuccessful? Just put original file back


  // Swap our original file and our temp file
  char swap_path[PATH_MAX];
  fullpath("/swap", swap_path); // We just need a path to swap, no file necessary here

  rename(db_path, swap_path); // log -> swap (keep ref to delete later)
  rename(temp_path, db_path); // temp -> log (what we wrote to is now the real log file)
  remove(swap_path);	       // delete swap (original log)


  // -------------------------------------------------------
  // -------------------------------------------------------
  // -------- STEP 2: WRITE ACTUAL DATA TO FILE ------------
  // -------------------------------------------------------
  // -------------------------------------------------------
  
  // Write data to desired file, then close it.
  pwrite (fi->fh, buf, size, off);

  //Keep the file descriptor of the file we're writing to.
  u_int64_t file_descriptor = fi->fh;

  // Close before opening other files
  close(fi->fh);

  close(lfd);
  remove(lock_path);

  //Return size as originally.
  return size;
}




int
ntapfuse_statfs (const char *path, struct statvfs *buf)
{
  func_log("statfs called\n");
  char fpath[PATH_MAX];
  fullpath (path, fpath);

  return statvfs (fpath, buf) ? -errno : 0;
}

int
ntapfuse_release (const char *path, struct fuse_file_info *fi)
{
  func_log("release called\n");
  return close (fi->fh) ? -errno : 0;
}

int
ntapfuse_fsync (const char *path, int datasync, struct fuse_file_info *fi)
{
  func_log("fsync called\n");
  if (datasync)
    return fdatasync (fi->fh) ? -errno : 0;
  else
    return fsync (fi->fh) ? -errno : 0;
}

int
ntapfuse_setxattr (const char *path, const char *name, const char *value,
	       size_t size, int flags)
{
  func_log("setxattr called\n");
  char fpath[PATH_MAX];
  fullpath (path, fpath);

  return lsetxattr (fpath, name, value, size, flags) ? -errno : 0;
}

int
ntapfuse_getxattr (const char *path, const char *name, char *value, size_t size)
{
  func_log("getxattr called\n");
  char fpath[PATH_MAX];
  fullpath (path, fpath);

  ssize_t s = lgetxattr (fpath, name, value, size);
  return s < 0 ? -errno : s;
}

int
ntapfuse_listxattr (const char *path, char *list, size_t size)
{
  func_log("listxattr called\n");
  char fpath[PATH_MAX];
  fullpath (path, fpath);

  return llistxattr (fpath, list, size) < 0 ? -errno : 0;
}

int
ntapfuse_removexattr (const char *path, const char *name)
{
  func_log("removexattr called\n");
  char fpath[PATH_MAX];
  fullpath (path, fpath);

  return lremovexattr (fpath, name) ? -errno : 0;
}

int
ntapfuse_opendir (const char *path, struct fuse_file_info *fi)
{
  func_log("opendir called\n");
  char fpath[PATH_MAX];
  fullpath (path, fpath);

  DIR *dir = opendir (fpath);
  if (dir == NULL)
    return -errno;

  fi->fh = (uint64_t) dir;

  return 0;
}

int
ntapfuse_readdir (const char *path, void *buf, fuse_fill_dir_t fill, off_t off,
	      struct fuse_file_info *fi)
{
  func_log("readdir called\n");
  struct dirent *de = NULL;

  while ((de = readdir ((DIR *) fi->fh)) != NULL)
    {
      struct stat st;
      memset (&st, 0, sizeof (struct stat));
      st.st_ino = de->d_ino;
      st.st_mode = de->d_type << 12;

      if (fill (buf, de->d_name, &st, 0))
	break;
    }

  return 0;
}

int
ntapfuse_releasedir (const char *path, struct fuse_file_info *fi)
{
  func_log("releasedir called\n");
  return closedir ((DIR *) fi->fh) ? -errno : 0;
}

int
ntapfuse_access (const char *path, int mode)
{
  func_log("access called\n");
  char fpath[PATH_MAX];
  fullpath (path, fpath);

  return access (fpath, mode) ? -errno : 0;
}

void *
ntapfuse_init (struct fuse_conn_info *conn)
{
  return (fuse_get_context())->private_data;
}
