#ifndef EXPRESSIONTREE_H
#define EXPRESSIONTREE_H

/* BASE EXPRESSION */
typedef char expressiontype;
typedef struct expression_t {
	expressiontype type;

} expression, *pexpression;
pll_entry syntax_parse_expression(pll_entry tokens, pexpression* expression);

/* LOCAL SPACE EXPRESSIONS */
#define EXPRESSION_ASSIGN		10
#define EXPRESSION_CALL			11
#define EXPRESSION_INCREMENT	12
#define EXPRESSION_DECREMENT	13

/* OTHER EXPRESSIONS */
typedef struct bracketexpression_t {
	expressiontype type;

	pexpression expression;
} bracketexpression, *pbracketexpression;
pll_entry syntax_parse_bracketexpression(pll_entry tokens, pbracketexpression* expression);

/* ARITHMETIC EXPRESSIONS */

#endif /* EXPRESSIONTREE_H */
