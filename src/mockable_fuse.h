#ifndef _FUSE_H_
#define _FUSE_H_

struct fuse_context {
	/** Pointer to the fuse object */
	struct fuse *fuse;

	/** User ID of the calling process */
	uid_t uid;

	/** Group ID of the calling process */
	gid_t gid;

	/** Thread ID of the calling process */
	pid_t pid;

	/** Private filesystem data */
	void *private_data;

	/** Umask of the calling process (introduced in version 2.8) */
	mode_t umask;
};

struct fuse_file_info {
	/** Open flags.	 Available in open() and release() */
	int flags;

	/** Old file handle, don't use */
	unsigned long fh_old;

	/** In case of a write operation indicates if this was caused by a
	    writepage */
	int writepage;

	/** Can be filled in by open, to use direct I/O on this file.
	    Introduced in version 2.4 */
	unsigned int direct_io : 1;

	/** Can be filled in by open, to indicate, that cached file data
	    need not be invalidated.  Introduced in version 2.4 */
	unsigned int keep_cache : 1;

	/** Indicates a flush operation.  Set in flush operation, also
	    maybe set in highlevel lock operation and lowlevel release
	    operation.	Introduced in version 2.6 */
	unsigned int flush : 1;

	/** Can be filled in by open, to indicate that the file is not
	    seekable.  Introduced in version 2.8 */
	unsigned int nonseekable : 1;

	/* Indicates that flock locks for this file should be
	   released.  If set, lock_owner shall contain a valid value.
	   May only be set in ->release().  Introduced in version
	   2.9 */
	unsigned int flock_release : 1;

	/** Padding.  Do not use*/
	unsigned int padding : 27;

	/** File handle.  May be filled in by filesystem in open().
	    Available in all other file operations */
	uint64_t fh;

	/** Lock owner id.  Available in locking operations and flush */
	uint64_t lock_owner;
};

struct utimbuf {
};

struct statvfs{
};


struct fuse_conn_info {
};

typedef int (*fuse_fill_dir_t) (void *buf, const char *name,
				void *stbuf, off_t off);

struct fuse_context *
fuse_get_context(void);

int truncate(const char *path, off_t length);
int unlink(const char *pathname); 
int rmdir(const char *pathname); 
int link(const char *oldpath, const char *newpath); 
int symlink(const char *oldpath, const char *newpath); 
int mknod(const char *pathname, mode_t mode, dev_t dev);
int mkdir(const char *pathname, mode_t mode);
ssize_t pwrite(int fd, const void *buf, size_t count, off_t offset); 
ssize_t pread(int fd, void *buf, size_t count, off_t offset); 
ssize_t write(int fd, const void *buf, size_t count); 
int chown(const char *path, uid_t owner, gid_t group);

#endif /*_FUSE_H_*/
