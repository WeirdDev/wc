#ifndef GLOBALTREE_H
#define GLOBALTREE_H

#include "expressiontree.h"
#include "localtree.h"

/* GLOBAL MEMBERS */
typedef char gsmembertype;
#define GSMEMBER_VARIABLE	1
#define GSMEMBER_FUNCTION	2
typedef struct gsmember_t {
	gsmembertype type;

} gsmember, *pgsmember;
extern pll_entry syntax_parse_gsmember(pll_entry tokens, pgsmember* ret);

typedef struct gsvariable_t {
	gsmember gsmember;

	char* name;
	pexpression expression;
} gsvariable, *pgsvariable;
extern pll_entry syntax_parse_gsvariable(pll_entry tokens, pgsvariable* ret);

typedef struct gsfunction_t {
	gsmember gsmember;

	char* name;
	plinkedlist parameters;
	plsblock localspace;
} gsfunction, *pgsfunction;
extern pll_entry syntax_parse_gsfunction(pll_entry tokens, pgsfunction* ret);

#endif /* GLOBALTREE_H */
