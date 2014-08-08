#ifndef EXPRESSIONTREE_H
#define EXPRESSIONTREE_H

#include "member.h"
#include "ll.h"

/* BASE EXPRESSION */
typedef struct expression_t {
	struct member_t base;

} expression, *pexpression;
pll_entry syntax_parse_expression(pll_entry tokens, pexpression* expression);

/* TERMINAL EXPRESSIONS */
#define EXPRESSION_NUMBER		EXTN(11)
#define EXPRESSION_STRING		EXTN(12)
#define EXPRESSION_NULL			EXTN(13)
#define EXPRESSION_CALL			EXTN(14)
extern pll_entry syntax_parse_exterminal(pll_entry tokens, pexpression* ret);

typedef struct exconstant_t {
	expression base;

	union { int number; char* string; void* null; } value;
} exconstant, *pexconstant;
extern pll_entry syntax_parse_exconstant(pll_entry tokens, pexconstant* ret);

typedef struct excall_t {
	expression base;

	char* identifier;
	plinkedlist arguments;
} excall, *pexcall;
extern pll_entry syntax_parse_excall(pll_entry tokens, pexconstant* ret);

/* UNARY EXPRESSIONS */
#define EXPRESSION_INCREMENT	EXTN(21)
#define EXPRESSION_DECREMENT	EXTN(22)
#define EXPRESSION_NEGATION		EXTN(23)
#define EXPRESSION_UNARYPLUS	EXTN(24)
#define EXPRESSION_UNARYMINUS	EXTN(25)
#define EXPRESSION_NOT			EXTN(26)
#define EXPRESSION_REFERENCE	EXTN(27)
typedef struct exunary_t {
	expression base;

	pexpression expression;
} exunary, *pexunary;
extern pll_entry syntax_parse_exunary(pll_entry tokens, pexconstant* ret);

/* ARITHMETIC EXPRESSIONS */
#define EXPRESSION_ADD			EXTN(30)
#define EXPRESSION_SUB			EXTN(31)
#define EXPRESSION_MUL			EXTN(32)
#define EXPRESSION_DIV			EXTN(33)
#define EXPRESSION_MOD			EXTN(34)
#define EXPRESSION_AND			EXTN(35)
#define EXPRESSION_OR			EXTN(36)
#define EXPRESSION_XOR			EXTN(37)
#define EXPRESSION_SHL			EXTN(38)
#define EXPRESSION_SHR			EXTN(39

/* LOGIC EXPRESSION */
#define EXPRESSION_EQUAL		EXTN(41)
#define EXPRESSION_NOTEQUAL		EXTN(42)
#define EXPRESSION_SMALLERTHAN	EXTN(43)
#define EXPRESSION_BIGGERTHAN	EXTN(44)
#define EXPRESSION_SMALLEREQUAL	EXTN(45)
#define EXPRESSION_BIGGEREQUAL	EXTN(46)
#define EXPRESSION_LOGICOR		EXTN(47)
#define EXPRESSION_LOGICAND		EXTN(48)
#define EXPRESSION_ASSIGN		EXTN(49)
typedef struct exbinary_t {
	expression base;

	pexpression left, right;
} exbinary, *pexbinary;
extern pll_entry syntax_parse_exbinary(pll_entry tokens, pexbinary* ret);

/* OTHER EXPRESSIONS */
#define EXPRESSION_TERNARY		EXTN(51)
typedef struct externary_t {
	expression base;

	pexpression cond, iftrue, iffalse;
} externary, *pexternary;
extern pll_entry syntax_parse_externary(pll_entry tokens, pexternary* ret);

#define EXPRESSION_BRACKETS		EXTN(52)
typedef struct bracketexpression_t {
	expression base;

	pexpression expression;
} bracketexpression, *pbracketexpression;
pll_entry syntax_parse_exbracket(pll_entry tokens, pbracketexpression* expression);

#define EXPRESSION_INDEX		EXTN(53)
typedef struct exindex_t {
	expression base;

	pexpression indexable, index;
} exindex, *pexindex;
extern pll_entry syntax_parse_exindex(pll_entry tokens, pexindex* ret);

/* ARITHMETIC EXPRESSIONS */

#endif /* EXPRESSIONTREE_H */
