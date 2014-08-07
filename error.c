#include <stdio.h>
#include <stdlib.h>
#include "error.h"

void error_fatal(char* msg) {
	error(msg);
	exit(EXIT_FAILURE);
}
void error(char* msg) {
	printf("ERROR: %s", msg);
}
void warning(char* msg) {
	printf("WARNING: %s", msg);
}