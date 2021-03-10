#ifndef _COMMON_H_
#define _COMMON_H_

#include <stdint.h>

// Default quota size is 1 MB
#define DEFAULT_QUOTA 1048576

typedef struct Record
{
	char  user[21];
	uint64_t total;
	uint64_t quota;
	uint32_t block;
} Record;

#endif /*_COMMON_H_*/
