#ifndef EXPRESSIONTREE_H
#define EXPRESSIONTREE_H

#include "member.h"
#include "ll.h"
#include "lexer.h"

/* BASE EXPRESSION */
typedef struct expression_t {
	struct member_t base;

} expression, *pexpression;
extern pll_entry syntax_parse_expression(pll_entry tokens, pexpression* expression);
extern pll_entry syntax_parse_expressionlist(pll_entry tokens, plinkedlist* expressions);

/* TERMINAL EXPRESSIONS */
#define EXPRESSION_NUMBER		EXTN(11)
#define EXPRESSION_STRING		EXTN(12)
#define EXPRESSION_NULL			EXTN(13)

#define EXPRESSION_VARIABLE		EXTN(14)
#define EXPRESSION_CALL			EXTN(15)
extern pll_entry syntax_parse_exterminal(pll_entry tokens, pexpression* ret);

typedef struct exconstant_t {
	expression base;

	union { int number; char* string; void* null; } value;
} exconstant, *pexconstant;
extern pll_entry syntax_parse_exconstant(pll_entry tokens, pexconstant* ret);

typedef struct exvariable_t {
	expression base;

	char* name;
} exvariable, *pexvariable;

typedef struct excall_t {
	expression base;

	char* identifier;
	plinkedlist arguments;
} excall, *pexcall;
extern pll_entry syntax_parse_excall(pll_entry tokens, pexcall* ret);

/* UNARY EXPRESSIONS */
#define EXPRESSION_PREFIXINC	EXTN(21)
#define EXPRESSION_PREFIXDEC	EXTN(22)
#define EXPRESSION_POSTFIXINC	EXTN(23)
#define EXPRESSION_POSTFIXDEC	EXTN(24)
#define EXPRESSION_NEGATION		EXTN(22)
#define EXPRESSION_UNARYPLUS	EXTN(23)
#define EXPRESSION_UNARYMINUS	EXTN(27)
#define EXPRESSION_NOT			EXTN(28)
#define EXPRESSION_REFERENCE	EXTN(29)
typedef struct exunary_t {
	expression base;

	pexpression expression;
} exunary, *pexunary;
extern pll_entry syntax_parse_exprefixunary(pll_entry tokens, pexunary* ret);

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
#define EXPRESSION_SHR			EXTN(39)

/* LOGIC EXPRESSION */
#define EXPRESSION_EQUAL		EXTN(41)
#define EXPRESSION_NOTEQUAL		EXTN(42)
#define EXPRESSION_SMALLERTHAN	EXTN(43)
#define EXPRESSION_BIGGERTHAN	EXTN(44)
#define EXPRESSION_SMALLEREQUAL	EXTN(45)
#define EXPRESSION_BIGGEREQUAL	EXTN(46)
#define EXPRESSION_LOGICOR		EXTN(47)
#define EXPRESSION_LOGICAND		EXTN(48)


/* ASSIGNMENT EXPRESSIONS */
#define EXPRESSION_ASSIGN		EXTN(50)
#define EXPRESSION_ASSIGNPLUS	EXTN(51)
#define EXPRESSION_ASSIGNMINUS	EXTN(52)
#define EXPRESSION_ASSIGNMUL	EXTN(53)
#define EXPRESSION_ASSIGNDIV	EXTN(54)
#define EXPRESSION_ASSIGNMOD	EXTN(55)
#define EXPRESSION_ASSIGNAND	EXTN(56)
#define EXPRESSION_ASSIGNOR		EXTN(57)
#define EXPRESSION_ASSIGNXOR	EXTN(58)
#define EXPRESSION_ASSIGNSHL	EXTN(59)
#define EXPRESSION_ASSIGNSHR	EXTN(60)
typedef struct exbinary_t {
	expression base;

	pexpression left, right;
} exbinary, *pexbinary;
extern pexbinary syntax_create_exbinary(ptoken operator, pexpression left, pexpression right);

/* OTHER EXPRESSIONS */
#define EXPRESSION_TERNARY		EXTN(71)
typedef struct externary_t {
	expression base;

	pexpression cond, iftrue, iffalse;
} externary, *pexternary;
extern pll_entry syntax_parse_externary(pll_entry tokens, pexternary* ret);

#define EXPRESSION_BRACKETS		EXTN(72)
typedef struct exbracket_t {
	expression base;

	pexpression expression;
} exbracket, *pexbracket;
pll_entry syntax_parse_exbracket(pll_entry tokens, pexbracket* expression);

#define EXPRESSION_INDEX		EXTN(73)
typedef struct exindex_t {
	expression base;

	pexpression indexable, index;
} exindex, *pexindex;
extern pll_entry syntax_parse_exindex(pll_entry tokens, pexindex* ret);

#define EXPRESSION_ARRAY		EXTN(74)
typedef struct exarray_t {
	expression base;

	plinkedlist items;
} exarray, *pexarray;
extern pll_entry syntax_parse_exarray(pll_entry tokens, pexarray* ret);

/* ARITHMETIC EXPRESSIONS */

#endif /* EXPRESSIONTREE_H */
