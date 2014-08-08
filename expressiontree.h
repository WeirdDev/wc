#ifndef EXPRESSIONTREE_H
#define EXPRESSIONTREE_H

#include "member.h"
#include "ll.h"

/* BASE EXPRESSION */
typedef struct expression_t {
	struct member_t base;

} expression, *pexpression;
pll_entry syntax_parse_expression(pll_entry tokens, pexpression* expression);

/* LOCAL SPACE EXPRESSIONS */
#define EXPRESSION_ASSIGN		EXTN(10)
#define EXPRESSION_CALL			EXTN(11)
#define EXPRESSION_INCREMENT	EXTN(12)
#define EXPRESSION_DECREMENT	EXTN(13)

/* OTHER EXPRESSIONS */
typedef struct bracketexpression_t {
	expression base;

	pexpression expression;
} bracketexpression, *pbracketexpression;
pll_entry syntax_parse_bracketexpression(pll_entry tokens, pbracketexpression* expression);

/* ARITHMETIC EXPRESSIONS */

#endif /* EXPRESSIONTREE_H */
