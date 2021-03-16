#ifndef _COMMON_H_
#define _COMMON_H_

#include <stdint.h>
#include <sys/types.h>

typedef struct Record
{
	uid_t    user_id;
	uint64_t total;
	uint64_t quota;
	uint32_t block;
} Record;

#endif /*_COMMON_H_*/
