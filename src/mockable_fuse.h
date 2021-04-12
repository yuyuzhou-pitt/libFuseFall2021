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

struct fuse_context *
fuse_get_context(void);

int unlink(const char *pathname); 
int link(const char *oldpath, const char *newpath); 

#endif /*_FUSE_H_*/
