#ifndef EXPRESSIONTREE_H
#define EXPRESSIONTREE_H

/* EXPRESSION BASE */
typedef char expressiontype;
typedef struct expression_t {
	expressiontype type;

} expression, *pexpression;
pll_entry syntax_parse_expression(pll_entry tokens, pexpression* expression);

/* ARITHMETIC EXPRESSIONS */

#endif /* EXPRESSIONTREE_H */
