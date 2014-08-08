#ifndef LOCALTREE_H
#define LOCALTREE_H

#include "ll.h"

#include "member.h"

/* LOCAL SPACE MEMBERS */
#define LSMEMBER_LSBLOCK		LSTN(1)
#define LSMEMBER_VARIABLE		LSTN(2)
#define LSMEMBER_EXPRESSION		LSTN(3) //<-- these are expressions like calls, assignments or (in/de)crementations
#define LSMEMBER_IF				LSTN(4)
#define LSMEMBER_ELSE			LSTN(5)
#define LSMEMBER_WHILE			LSTN(6)
#define LSMEMBER_RETURN			LSTN(7)

typedef struct lsmember_t {
	member base;

} lsmember, *plsmember;
extern pll_entry syntax_parse_lsmember(pll_entry tokens, plsmember* ret);

typedef struct lsblock_t {
	lsmember base;

	plinkedlist localspace;
} lsblock, *plsblock;
extern plsblock syntax_create_lsblock(plinkedlist members);
extern pll_entry syntax_parse_lsblock(pll_entry tokens, plsblock* localspace);
extern pll_entry syntax_parse_lsblock_singlemember(pll_entry tokens, plsblock* localspace);

typedef struct lsvariable_t {
	lsmember base;

	char* name;
	pexpression expression;
} lsvariable, *plsvariable;
extern pll_entry syntax_parse_lsvariable(pll_entry tokens, plsvariable* ret);

typedef struct lsexpression_t {
	lsmember base;

	pexpression expression;
} lsexpression, *plsexpression;
extern pll_entry syntax_parse_lsexpression(pll_entry tokens, plsexpression* ret);

typedef struct lswhile_t {
	lsmember base;

	pexpression expression;
	plsblock localspace;
} lswhile, *plswhile;
extern pll_entry syntax_parse_lswhile(pll_entry tokens, plswhile* ret);

typedef struct lselse_t {
	lsmember base;

	plsblock localspace;
} lselse, *plselse;
extern pll_entry syntax_parse_lselse(pll_entry tokens, plselse* ret);

typedef struct lsif_t {
	lsmember base;

	pexpression expression;
	plsblock localspace;

	plselse elseblock;
} lsif, *plsif;
extern pll_entry syntax_parse_lsif(pll_entry tokens, plsif* ret);

typedef struct lsreturn_t {
	lsmember base;
	pexpression expression;
} lsreturn, *plsreturn;
extern pll_entry syntax_parse_lsreturn(pll_entry tokens, plsreturn* ret);

#endif
