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
#include "business_logic.h"
#include <stdio.h>
#include <stdarg.h>

#include <errno.h>
#include <dirent.h>
#include <limits.h>
#include <string.h>
#include <unistd.h>

#include <sys/xattr.h>
#include <sys/types.h>

#include <sys/stat.h>

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

off_t
get_filesize(const char *path){
  struct stat sb;
  int re = stat(path, &sb);
  return (re == -1)? -1 : sb.st_size;
}

uid_t
get_owner(const char *path){
  struct stat sb;
  int re = stat(path, &sb);
  if(re != -1){
    if(ensure_user_exists(sb.st_uid) == 0){
      return sb.st_uid;
    }
  }

  return -1;
}

uid_t
get_owner_fd(int fd){
  struct stat sb;
  int re = fstat(fd, &sb);
  if(re != -1){
    if(ensure_user_exists(sb.st_uid) == 0){
      return sb.st_uid;
    }
  }

  return -1;
}

void
log_data(const char * format, ...){
    
  char fpath[PATH_MAX];
  const char* lpath = "/../log.txt";
  fullpath (lpath, fpath);
  FILE * fp = fopen(fpath, "a");    

  va_list args;
  va_start(args, format);
  vfprintf(fp, format, args);
  va_end(args);

  fclose(fp);
}

/* The following functions describe FUSE operations. Each operation appends
   the path of the root filesystem to the given path in order to give the
   mirrored path. */

int
ntapfuse_getattr (const char *path, struct stat *buf)
{
  char fpath[PATH_MAX];
  fullpath (path, fpath);

  return lstat (fpath, buf) ? -errno : 0;
}

int
ntapfuse_readlink (const char *path, char *target, size_t size)
{
  char fpath[PATH_MAX];
  fullpath (path, fpath);

  return readlink (fpath, target, size) < 0 ? -errno : 0;
}


int
ntapfuse_mknod (const char *path, mode_t mode, dev_t dev)
{
  char fpath[PATH_MAX];
  fullpath (path, fpath);

  // Need to create file to get the uid later.
  // I don't like this.
  int re = mknod (fpath, mode, dev);
  if(re){
    log_data("mknod: err %d\n", re);
    return -errno;
  }

  long uid = get_owner(fpath);
  if(update_file_record(uid, 1)){
    log_data("mknod: \n\tPATH: %s \t %s\n\tOWNER: %zu\n", path, fpath, uid);
    print_all();
    return 0;
  }
  else{
    // User's inode quota has been reached
    log_data("INODE QUOTA has been reached!\n");
    unlink(fpath);
    return -EDQUOT;
  }
}

int
ntapfuse_mkdir (const char *path, mode_t mode)
{
  char fpath[PATH_MAX];
  fullpath (path, fpath);

  // Need to create file to get the uid later.
  // I don't like this.
  int re = mkdir(fpath, mode | S_IFDIR);
  if(re){
    log_data("mknod: err %d\n", re);
    return -errno;
  }

  long uid = get_owner(fpath);
  if(update_file_record(uid, 1)){
    log_data("mkdir: \n\tPATH: %s\n\tOWNER: %zu\n", path, uid);
    print_all();
    return 0;
  }
  else{
    // User's inode quota has been reached
    log_data("INODE QUOTA has been reached!\n");
    rmdir(fpath);
    return -EDQUOT;
  }
}

int
ntapfuse_unlink (const char *path)
{
  char fpath[PATH_MAX];
  fullpath (path, fpath);

  long uid = get_owner(fpath);
  long fsize = get_filesize(fpath);

  if(unlink (fpath) == 0){
    update_file_record(uid, -1);
    update_usage_record(uid, -fsize);
    log_data("unlink: \n\tPATH: %s\n\tOWNER: %zu\n\tSIZE: %zu\n", path, uid, fsize);
    print_all();
    return 0;
  }
  else {
    return -errno;
  }
}

int
ntapfuse_rmdir (const char *path)
{
  char fpath[PATH_MAX];
  fullpath (path, fpath);

  int uid = get_owner(fpath);
  if(rmdir(fpath) == 0){
    update_file_record(uid, -1);
    log_data("rmdir: \n\tPATH: %s\n\tOWNER: %zu\n", path, uid);
    print_all();
    return 0;
  }
  else {
    return -errno;
  }
}

//Currently wrong
int
ntapfuse_symlink (const char *path, const char *link)
{
  char flink[PATH_MAX];
  fullpath (link, flink);

  long uid = get_owner(flink);
  if(update_file_record(uid, 1)){
    log_data("symlink: \n\tPATH: %s\n\tOWNER: %zu\n", path, uid);
    print_all();
    int re = symlink (path, flink) ? -errno : 0;
    if(re){
      update_file_record(uid, -1);
      return -errno;
    }    
    return 0;
  }
  else{
    // User's inode quota has been reached
    log_data("INODE QUOTA has been reached!\n");
    return -EDQUOT;
  }
}

int
ntapfuse_rename (const char *src, const char *dst)
{
  char fsrc[PATH_MAX];
  fullpath (src, fsrc);

  char fdst[PATH_MAX];
  fullpath (dst, fdst);

  return rename (fsrc, fdst) ? -errno : 0;
}

// Should we track this? I don't know...
int
ntapfuse_link (const char *src, const char *dst)
{
  char fsrc[PATH_MAX];
  fullpath (src, fsrc);

  char fdst[PATH_MAX];
  fullpath (dst, fdst);

  return link (fsrc, fdst) ? -errno : 0;
}

int
ntapfuse_chmod (const char *path, mode_t mode)
{
  char fpath[PATH_MAX];
  fullpath (path, fpath);

  return chmod (fpath, mode) ? -errno : 0;
}

// NOT FULLY IMPLEMENTED OR TESTED
int
ntapfuse_chown (const char *path, uid_t uid, gid_t gid)
{
  char fpath[PATH_MAX];
  fullpath (path, fpath);

  off_t size = get_filesize(fpath);

  if(update_usage_record(uid, size)){
    int re = chown (fpath, uid, gid);
    if(re < 0){
      update_usage_record(uid, -size);
      return -errno;
    }

    log_data("CHOWN: \n\tPATH: %s\n\tSIZE: %lu\n\tUID: %lu\n", path, size, uid);
    print_all();
    return re;
  }
  else {
    // User's disk quota has been reached
    log_data("QUOTA has been reached!\n");
    return -EDQUOT;
  }
}

int
ntapfuse_truncate (const char *path, off_t off)
{
  char fpath[PATH_MAX];
  fullpath (path, fpath);

  int uid = get_owner(fpath);
  int fsize = get_filesize(fpath);
  if(truncate (fpath, off) == 0){
    log_data("truncate: \n\tPATH: %s\n\tSIZE: %d\n", path, off - fsize);
    print_all();
    update_usage_record(uid, off - fsize);
    return 0;
  }
  else {
    return -errno;
  }
}

int
ntapfuse_utime (const char *path, struct utimbuf *buf)
{
  char fpath[PATH_MAX];
  fullpath (path, fpath);

  return utime (fpath, buf) ? -errno : 0;
}

int
ntapfuse_open (const char *path, struct fuse_file_info *fi)
{
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
  return pread (fi->fh, buf, size, off) < 0 ? -errno : size;
}

int
ntapfuse_write (const char *path, const char *buf, size_t size, off_t off,
	    struct fuse_file_info *fi)
{
  char fpath[PATH_MAX];
  fullpath (path, fpath);

  int uid = get_owner_fd(fi->fh);

  if(update_usage_record(uid, size)){
    int return_size = pwrite(fi->fh, buf, size, off);
    if(return_size < 0){
      update_usage_record(uid, -size);
      return -errno;
    }

    log_data("write: \n\tPATH: %s\n\tSIZE: %lu\n\tOFFS: %lu\n", path, return_size, off);
    print_all();
    update_usage_record(uid, return_size - size);
    return return_size;
  }
  else {
    // User's disk quota has been reached
    log_data("QUOTA has been reached!\n");
    return -EDQUOT;
  }
}

int
ntapfuse_statfs (const char *path, struct statvfs *buf)
{
  char fpath[PATH_MAX];
  fullpath (path, fpath);

  return statvfs (fpath, buf) ? -errno : 0;
}

int
ntapfuse_release (const char *path, struct fuse_file_info *fi)
{
  return close (fi->fh) ? -errno : 0;
}

int
ntapfuse_fsync (const char *path, int datasync, struct fuse_file_info *fi)
{
  if (datasync)
    return fdatasync (fi->fh) ? -errno : 0;
  else
    return fsync (fi->fh) ? -errno : 0;
}

int
ntapfuse_setxattr (const char *path, const char *name, const char *value,
	       size_t size, int flags)
{
  char fpath[PATH_MAX];
  fullpath (path, fpath);

  return lsetxattr (fpath, name, value, size, flags) ? -errno : 0;
}

int
ntapfuse_getxattr (const char *path, const char *name, char *value, size_t size)
{
  char fpath[PATH_MAX];
  fullpath (path, fpath);

  ssize_t s = lgetxattr (fpath, name, value, size);
  return s < 0 ? -errno : s;
}

int
ntapfuse_listxattr (const char *path, char *list, size_t size)
{
  char fpath[PATH_MAX];
  fullpath (path, fpath);

  return llistxattr (fpath, list, size) < 0 ? -errno : 0;
}

int
ntapfuse_removexattr (const char *path, const char *name)
{
  char fpath[PATH_MAX];
  fullpath (path, fpath);

  return lremovexattr (fpath, name) ? -errno : 0;
}

int
ntapfuse_opendir (const char *path, struct fuse_file_info *fi)
{
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
  return closedir ((DIR *) fi->fh) ? -errno : 0;
}

int
ntapfuse_access (const char *path, int mode)
{
  char fpath[PATH_MAX];
  fullpath (path, fpath);

  return access (fpath, mode) ? -errno : 0;
}

void *
ntapfuse_init (struct fuse_conn_info *conn)
{
  db_init(":memory:");
  return (fuse_get_context())->private_data;
}

