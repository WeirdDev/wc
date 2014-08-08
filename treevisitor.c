#include <malloc.h>
#include "treevisitor.h"

ptreevisitor syntax_treevisitor_new() {
	ptreevisitor ret = (ptreevisitor)calloc(sizeof(ptreevisitor), 1);
	return ret;
}
void syntax_treevisitor_visitglobalspace(ptreevisitor visitor, psyntaxtree tree) {
	pll_entry curr = tree->globalspace->first;
	while(curr != NULL) {
		switch(((pgsmember)curr->data)->type) {
			case GSMEMBER_VARIABLE:
				gsvariable_accept(visitor, (pgsvariable)curr->data);
				break;
			case GSMEMBER_FUNCTION:
				gsvariable_accept(visitor, (pgsfunction)curr->data);
				break;
			default:
				FATAL("Unknown gstype %d", ((pgsmember)curr->data)->type);
				break;
		}
	}
}
