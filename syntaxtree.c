#include "error.h"
#include "lexer.h"

#include "syntaxtree.h"
#include "globaltree.h"

psyntaxtree syntax_parse(plinkedlist tokens) {
	plinkedlist globalspace = ll_new();

	pll_entry centry = tokens->first;
	while(centry != NULL) {
		pgsmember member;

		centry = syntax_parse_gsmember(centry, &member);
		centry = centry->next;
		ll_push(globalspace, member);
	}

	psyntaxtree st = (psyntaxtree)malloc(sizeof(syntaxtree));
	st->globalspace = globalspace;
	return st;
}
