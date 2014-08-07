#include "error.h"
#include "lexer.h"

#include "syntax.h"
#include "globaltree.h"

plinkedlist syntax_parse(plinkedlist tokens) {
	plinkedlist members = ll_new();

	pll_entry centry = tokens->first;
	while(centry != NULL) {
		pgsmember member;

		centry = syntax_parse_gsmember(centry, &member);
		centry = centry->next;
		ll_push(members, member);
	}

	return members;
}
