#ifndef TREEVISITOR_H
#define TREEVISITOR_H

#include "syntaxtree.h"

struct treevisitor_t;
#define visitor_method(name)	void* (* visit##name )(struct treevisitor_t* visitor, p##name object)
typedef struct treevisitor_t {
	visitor_method(gsvariable);
	visitor_method(gsfunction);

	visitor_method(lsblock);
	visitor_method(lsvariable);
	visitor_method(lsexpression);
	visitor_method(lsif);
	visitor_method(lselse);
	visitor_method(lswhile);
} treevisitor, *ptreevisitor;
extern ptreevisitor syntax_treevisitor_new();
extern void syntax_treevisitor_visitglobalspace(ptreevisitor visitor, psyntaxtree tree);

#endif /* TREEVISITOR_H */
