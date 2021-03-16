#ifndef _COMMON_H_
#define _COMMON_H_

#include <stdint.h>
#include <stdarg.h>


//// Default quota size is 1 MB
// #define DEFAULT_USAGE_QUOTA 1048576
#define DEFAULT_USAGE_QUOTA 100
// The default user can create 1000 files
#define DEFAULT_FILE_QUOTA 1000

// Printf to log file. Implemented in ntapfuse_ops.c
void log_data(const char * format, ...);

#endif /*_COMMON_H_*/
