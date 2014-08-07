#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		exit(1);
	}

	FILE *fp;
	fp = fopen(argv[1], "r");

	if (fp == NULL)
	{
		exit(1);
	}
	
	return 0;
}