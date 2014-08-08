#ifndef SYNTAX_H
#define SYNTAX_H

#include "ll.h"

#include "member.h"

#include "globaltree.h"
#include "expressiontree.h"

typedef struct syntaxtree_t {
	member base;
	plinkedlist globalspace;

} syntaxtree, *psyntaxtree;

extern psyntaxtree syntaxtree_parse(plinkedlist tokens);

#endif /* SYNTAX_H */
