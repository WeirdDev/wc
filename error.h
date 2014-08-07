#ifndef ERROR_H
#define ERROR_H

#include <stdio.h>
#include <stdlib.h>

#define ERROR(...)		{ printf("ERROR: "); printf(__VA_ARGS__); }
#define FATAL(...)		{ printf("FATAL: "); printf(__VA_ARGS__); exit(EXIT_FAILURE); }
#define WARNING(...)	{ printf("WARNING: "); printf(__VA_ARGS__); }

#endif
