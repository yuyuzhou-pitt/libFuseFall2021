#ifndef _COMMON_H_
#define _COMMON_H_

#include <stdint.h>

typedef struct Record
{
	char  user[20];
	uint64_t total;
	uint64_t quota;
	uint32_t block;
} Record;

#endif /*_COMMON_H_*/
