#include <stdio.h>
#include <stdlib.h>

#include "error.h"

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		FATAL("No input files");
	}

	FILE *fp = fopen(argv[1], "r");
	if (fp == NULL)
	{
		FATAL("Could not open the file specified");
	}
	


	fclose(fp);
	return 0;
}
