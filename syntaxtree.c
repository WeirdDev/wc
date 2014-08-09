#include "error.h"
#include "lexer.h"

#include "syntaxtree.h"
#include "treevisitor.h"
#include "globaltree.h"

accept_method(syntaxtree) {
	visitor->visit(visitor, pmemconv(object));

	pll_entry curr = ((psyntaxtree)object)->globalspace->first;
	while(curr != NULL) {
		pmemconv(curr->data)->accept(visitor, pmemconv(curr->data));

		curr = curr->next;
	}
}
psyntaxtree syntaxtree_parse(plinkedlist tokens) {
	plinkedlist globalspace = ll_new();

	pll_entry centry = tokens->first;
	while(centry != NULL) {
		pgsmember member;

		centry = syntax_parse_gsmember(centry, &member);
		centry = centry->next;
		ll_push(globalspace, member);
	}

	psyntaxtree st = member_new(syntaxtree);
	st->globalspace = globalspace;
	st->base.accept = &syntaxtree_accept;
	return st;
}

