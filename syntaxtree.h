#ifndef SYNTAX_H
#define SYNTAX_H

#include "ll.h"

#include "globaltree.h"
#include "expressiontree.h"

typedef struct syntaxtree_t {
	plinkedlist globalspace;

} syntaxtree, *psyntaxtree;

extern psyntaxtree syntax_parse(plinkedlist tokens);

#endif /* SYNTAX_H */
