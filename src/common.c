#include <stdio.h>
#include <stdarg.h>

#include "common.h"

void log_data(const char *format, ...){
	FILE * fp = fopen("log.txt", "a");    
	if (fp == NULL)
		return;
	
	va_list args;
	va_start(args, format);
	vfprintf(fp, format, args);
	va_end(args);

	fclose(fp);
}
