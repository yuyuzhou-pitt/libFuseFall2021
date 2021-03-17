#ifndef _COMMON_H_
#define _COMMON_H_

#include <stdint.h>
#include <sys/types.h>

// Default byte quota size is 1 MB or 2^20 bytes
#define DEFAULT_BYTE_QUOTA 1<<20

// Default file quota size is 100
#define DEFAULT_FILE_QUOTA 100

typedef struct Record
{
	uid_t    user_id;
	uint64_t byte_total;
	uint64_t byte_quota;
	uint64_t file_total;
	uint64_t file_quota;
	uint32_t block;
} Record;

#endif /*_COMMON_H_*/
