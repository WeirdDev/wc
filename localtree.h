#ifndef LOCALTREE_H
#define LOCALTREE_H

#include "ll.h"

typedef char lsmembertype;
/* LOCAL SPACE MEMBERS */
#define LSMEMBER_VARIABLE		1
#define LSMEMBER_EXPRESSION		2 //<-- these are expressions like calls, assignments or (in/de)crementations
#define LSMEMBER_IF				3
#define LSMEMBER_ELSE			4
#define LSMEMBER_WHILE			5

typedef struct lsmember_t {
	lsmembertype type;

} lsmember, *plsmember;
extern pll_entry syntax_parse_lsblock(pll_entry tokens, plinkedlist* members);
extern pll_entry syntax_parse_lsmember(pll_entry tokens, plsmember* ret);

typedef struct lsvariable {
	lsmember base;

	char* name;
	pexpression expression;
} lsvariable, *plsvariable;
extern pll_entry syntax_parse_lsvariable(pll_entry tokens, plsvariable* ret);

typedef struct lsexpression {
	lsmember base;

	pexpression expression;
} lsexpression, *plsexpression;
extern pll_entry syntax_parse_lsexpression(pll_entry tokens, plsexpression* ret);

typedef struct lswhile {
	lsmember base;

	pexpression expression;
	plinkedlist localspace;
} lswhile, *plswhile;
extern pll_entry syntax_parse_lswhile(pll_entry tokens, plswhile* ret);

typedef struct lselse {
	lsmember base;

	plinkedlist localspace;
} lselse, *plselse;

typedef struct lsif {
	lsmember base;

	pexpression expression;
	plinkedlist localspace;

	plselse elseblock;
} lsif, *plsif;
extern pll_entry syntax_parse_lsif(pll_entry tokens, plsif* ret);

extern pll_entry syntax_parse_lselse(pll_entry tokens, plselse* ret);

#endif
