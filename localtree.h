#ifndef LOCALTREE_H
#define LOCALTREE_H

#include "ll.h"

typedef char lsmembertype;
/* LOCAL SPACE MEMBERS */
#define LSMEMBER_VARIABLE		1
#define LSMEMBER_LSEXPRESSION	2 //<-- these are expressions like calls, assignments or (in/de)crementations
#define LSMEMBER_IF				3
#define LSMEMBER_ELSE			4
#define LSMEMBER_WHILE			5

typedef struct lsmember_t {
	lsmembertype type;

} lsmember, *plsmember;
pll_entry syntax_parse_lsblock(pll_entry tokens, plinkedlist* members);
pll_entry syntax_parse_lsmember(pll_entry tokens, plsmember* ret);

typedef struct lsvariable {
	lsmember base;

	char* name;
	pexpression expression;
} lsvariable, *plsvariable;

typedef struct lsexpression {
	lsmember base;

	pexpression expression;
} lsexpression, *plsexpression;

typedef struct lscondblock {
	lsmember base;

	pexpression expression;
	plinkedlist localspace;
} lscondblock, *plscondblock;

typedef struct lselse {
	lsmember base;

	plinkedlist localspace;
} lselse, *plselse;

#endif
