#include <malloc.h>

#include "error.h"

#include "globaltree.h"
#include "treevisitor.h"

void syntax_treevisitor_visit(struct treevisitor_t* visitor, pmember object);
ptreevisitor syntax_treevisitor_new() {
	ptreevisitor ret = (ptreevisitor)calloc(sizeof(ptreevisitor), 1);
	ret->visit = &syntax_treevisitor_visit;
	return ret;
}
void syntax_treevisitor_visit(struct treevisitor_t* visitor, pmember object) {
	switch(object->type) {
		case GSMEMBER_VARIABLE:
			visitor->gsvariable(visitor, (pgsvariable)object);
			break;
		case GSMEMBER_FUNCTION:
			visitor->gsfunction(visitor, (pgsfunction)object);
			break;

		//TODO: add other types
		default:
			FATAL("Unknown gstype %d", object->type);
			break;
	}
}
