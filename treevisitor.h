#ifndef TREEVISITOR_H
#define TREEVISITOR_H

#include "member.h"
#include "syntaxtree.h"

#include "ll.h"
#include "globaltree.h"
#include "localtree.h"
#include "expressiontree.h"

struct treevisitor_t;
#define visitor_method(name)		void visit##name (struct treevisitor_t* visitor, p##name object)
#define visitor_method_ptr(name)	void* (* visit##name )(struct treevisitor_t* visitor, p##name object)
#define accept_method(name)			void name##_accept(struct treevisitor_t* visitor, p##name object)
typedef struct treevisitor_t {
	void (*visit)(struct treevisitor_t* visitor, pmember object);

	visitor_method_ptr(syntaxtree);
	visitor_method_ptr(gsvariable);
	visitor_method_ptr(gsfunction);

	visitor_method_ptr(lsblock);
	visitor_method_ptr(lsvariable);
	visitor_method_ptr(lsexpression);
	visitor_method_ptr(lsif);
	visitor_method_ptr(lselse);
	visitor_method_ptr(lswhile);
} treevisitor, *ptreevisitor;
extern ptreevisitor syntax_treevisitor_new();
extern void visit(struct treevisitor_t* visitor, pmember object);

#endif /* TREEVISITOR_H */
