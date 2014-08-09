#ifndef LEXER_H
#define LEXER_H

#include "member.h"

/* EOF - End Of File */
#define TOKEN_EOF			TKTN(0)
/* EOL - End Of List */
#define TOKEN_EOL			TKTN(1)
/* Identifier */
#define TOKEN_WORD			TKTN(10)
/* Keywords */
#define TOKEN_VAR			TKTN(20)
#define TOKEN_FUNCTION		TKTN(21)
#define TOKEN_IF			TKTN(22)
#define TOKEN_ELSE			TKTN(23)
#define TOKEN_WHILE			TKTN(24)
#define TOKEN_RETURN		TKTN(25)
/* Constants */
#define TOKEN_NUMBER		TKTN(30)
#define TOKEN_STRING		TKTN(31)
#define TOKEN_NULL			TKTN(32)
/* Arithmetic operators */
#define TOKEN_PLUS			TKTN(40)
#define TOKEN_MINUS			TKTN(41)
#define TOKEN_MUL			TKTN(42)
#define TOKEN_DIV			TKTN(43)
#define TOKEN_MOD			TKTN(44)
/* Logic operators */
#define TOKEN_EQUAL			TKTN(50)
#define TOKEN_NOTEQUAL		TKTN(51)
#define TOKEN_SMALLERTHAN	TKTN(52)
#define TOKEN_BIGGERTHAN	TKTN(53)
#define TOKEN_SMALLEREQUAL	TKTN(54)
#define TOKEN_BIGGEREQUAL	TKTN(55)
#define TOKEN_NEGATION		TKTN(56)
#define TOKEN_LOGICAND		TKTN(57)
#define TOKEN_LOGICOR		TKTN(58)
/* Assignment operators */
#define TOKEN_ASSIGNPLUS	TKTN(60)
#define TOKEN_ASSIGNMINUS	TKTN(61)
#define TOKEN_ASSIGNMUL		TKTN(62)
#define TOKEN_ASSIGNDIV		TKTN(63)
#define TOKEN_ASSIGNMOD		TKTN(64)
#define TOKEN_ASSIGNAND		TKTN(65)
#define TOKEN_ASSIGNOR		TKTN(66)
#define TOKEN_ASSIGNXOR		TKTN(67)
#define TOKEN_ASSIGNSHL		TKTN(68)
#define TOKEN_ASSIGNSHR		TKTN(69)
/* Bitwise operators */
#define TOKEN_AND			TKTN(70)
#define TOKEN_OR			TKTN(71)
#define TOKEN_XOR			TKTN(72)
#define TOKEN_NOT			TKTN(73)
#define TOKEN_SHL			TKTN(74)
#define TOKEN_SHR			TKTN(75)
/* Other operators */
#define TOKEN_ASSIGN		TKTN(80)
#define TOKEN_INCREMENT		TKTN(81)
#define TOKEN_DECREMENT		TKTN(82)
/* Misc */
#define TOKEN_COLON			TKTN(90)
#define TOKEN_QMARK			TKTN(91)
#define TOKEN_SEMICOLON		TKTN(92)
#define TOKEN_COMMA			TKTN(93)
#define TOKEN_CBRSTART		TKTN(94)
#define TOKEN_CBREND		TKTN(95)
#define TOKEN_BRSTART		TKTN(96)
#define TOKEN_BREND			TKTN(97)
#define TOKEN_IBRSTART		TKTN(98)
#define TOKEN_IBREND		TKTN(99)

typedef struct token_t {
	member base;
	char* string;
} token, *ptoken;

extern token keywords[];
extern token operators[];

#include "ll.h"
#include <malloc.h>
extern plinkedlist lexer_parse(char* source);
extern pll_entry lexer_parse_brwords(pll_entry tokens, plinkedlist* list);

#define GETTKN(tokens)	((ptoken)tokens->data)
#define NEXTTKN(tokens)	(tokens->next)

#define UNEXP_TOKEN(ptoken)					{ FATAL("Unexpected token on line %d: '%s'", ptoken->base.line, ptoken->string); }
#define UNEXP_EXP_TOKEN(ptoken, expected)	{ FATAL("Unexpected token '%s' on line %d, expected '%d'", ptoken->string, ptoken->base.line, expected ); }

#define CHECK_TOKEN(ptoken, exptype)		if((ptoken)->base.type != exptype) { UNEXP_EXP_TOKEN(ptoken, exptype); }

extern ptoken lexer_token_create(membertype type, char* string, int line);
extern ptoken lexer_token_copy(ptoken src, int line);

/* for member.h */
/* and extended version of member_set(), where last argument (line number) is replaced with token (that contains the line number information) */
extern void member_sett(void* member, membertype type, accept_method_ptr accept, ptoken t);

#endif /* LEXER_H */
