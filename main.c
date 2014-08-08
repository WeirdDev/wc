#include <stdio.h>
#include <stdlib.h>

#include "error.h"
#include "ll.h"

#include "lexer.h"
#include "syntaxtree.h"
#include "treevisitor.h"

int main(int argc, char *argv[]) {
	/* check if we've got just enough input */
	if (argc != 2) {
		FATAL("No input files");
	}

	/* open the source file for reading */
	FILE *fp;
	if ((fp = fopen(argv[1], "rb")) == NULL) {
		FATAL("Could not open the file specified");
	}
	
	/* get the length of the file */
	fseek(fp, 0L, SEEK_END);
	long int fsize = ftell(fp);
	rewind(fp);

	/* read the file`s sources to memory */
	char* source = (char*)malloc(fsize+1);
	fread(source, sizeof(char), fsize, fp);
	source[fsize] = '\0';

	/* LEXICAL ANALYSIS */
	plinkedlist tokens = lexer_parse(source);

	/* FOR DEBUGGING PURPOSES ONLY */
	int length, i = 0;
	ptoken* array = (ptoken*)malloc(sizeof(ptoken) * (length = ll_count(tokens)));
	pll_entry curr = tokens->first;
	while(curr != NULL) {
		array[i++] = (ptoken)curr->data;
		curr = curr->next;
	}
	/* ----------------------- */

	/* SYNTACTICAL ANALYSIS */
	psyntaxtree syntaxtree = syntaxtree_parse(tokens);

	fclose(fp);
	return 0;
}
