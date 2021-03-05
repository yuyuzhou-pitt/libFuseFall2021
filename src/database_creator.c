#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "database_interface.h"

int main(int argc, char **argv){
	FILE *fp;
	fp = fopen("database.csv", "w");
	if (fp == NULL)
		return 1;

	fprintf(fp, "%-20.20s,%20.20i,%20.20i\n", "user1", 0, 100);
	fprintf(fp, "%-20.20s,%20.20i,%20.20i\n", "user2", 50, 100);	
	fprintf(fp, "%-20.20s,%20.20i,%20.20i\n", "user3", 0, 200);	
}
