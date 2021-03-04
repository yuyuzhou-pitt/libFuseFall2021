#ifndef _COMMON_H_
#define _COMMON_H_

#include <stdint.h>

typedef struct {
	char[30] user;
	uint64_t total;
	uint64_t quota;
	uint32_t block;
} record;

#endif /*_COMMON_H_*/
