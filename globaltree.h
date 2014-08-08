#ifndef GLOBALTREE_H
#define GLOBALTREE_H

#include "member.h"

#include "expressiontree.h"
#include "localtree.h"

/* GLOBAL MEMBERS */
#define GSMEMBER_VARIABLE	GSTN(1)
#define GSMEMBER_FUNCTION	GSTN(2)
typedef struct gsmember_t {
	member base;

} gsmember, *pgsmember;
extern pll_entry syntax_parse_gsmember(pll_entry tokens, pgsmember* ret);
extern void gsvariable_accept();

typedef struct gsvariable_t {
	gsmember base;

	char* name;
	pexpression expression;
} gsvariable, *pgsvariable;
extern pll_entry syntax_parse_gsvariable(pll_entry tokens, pgsvariable* ret);

typedef struct gsfunction_t {
	gsmember base;

	char* name;
	plinkedlist parameters;
	plsblock localspace;	/* <-- if NULL, this is a function declaration */
} gsfunction, *pgsfunction;
extern pll_entry syntax_parse_gsfunction(pll_entry tokens, pgsfunction* ret);

#endif /* GLOBALTREE_H */
