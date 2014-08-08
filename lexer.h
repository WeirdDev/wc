#ifndef LEXER_H
#define LEXER_H

#include "member.h"

/* EOF */
#define TOKEN_EOF			TKTN(0)
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
/* Bitwise operators */
#define TOKEN_AND			TKTN(60)
#define TOKEN_OR			TKTN(61)
#define TOKEN_XOR			TKTN(62)
#define TOKEN_NOT			TKTN(63)
#define TOKEN_SHL			TKTN(64)
#define TOKEN_SHR			TKTN(65)
/* Other operators */
#define TOKEN_ASSIGN		TKTN(70)
#define TOKEN_INCREMENT		TKTN(71)
#define TOKEN_DECREMENT		TKTN(72)
/* Misc */
#define TOKEN_COLON			TKTN(80)
#define TOKEN_QMARK			TKTN(81)
#define TOKEN_SEMICOLON		TKTN(82)
#define TOKEN_COMMA			TKTN(83)
#define TOKEN_CBRSTART		TKTN(84)
#define TOKEN_CBREND		TKTN(85)
#define TOKEN_BRSTART		TKTN(86)
#define TOKEN_BREND			TKTN(87)
#define TOKEN_IBRSTART		TKTN(88)
#define TOKEN_IBREND		TKTN(89)

typedef struct token_t {
	member base;
	char* string;
	
	int line;
} token, *ptoken;

extern token keywords[];
extern token operators[];

#include "ll.h"
#include <malloc.h>
extern plinkedlist lexer_parse(char* source);
extern pll_entry lexer_parse_brwords(pll_entry tokens, plinkedlist* list);

#define GETTKN(tokens)	((ptoken)tokens->data)
#define NEXTTKN(tokens)	(tokens->next)

#define UNEXP_TOKEN(ptoken)					{ FATAL("Unexpected token on line %d: '%s'", ptoken->line, ptoken->string); }
#define UNEXP_EXP_TOKEN(ptoken, expected)	{ FATAL("Unexpected token '%s' on line %d, expected '%d'", ptoken->string, ptoken->line, expected ); }

#define CHECK_TOKEN(ptoken, exptype)		if((ptoken)->base.type != exptype) { UNEXP_EXP_TOKEN(ptoken, exptype); }

extern ptoken lexer_token_create(membertype type, char* string, int line);
extern ptoken lexer_token_copy(ptoken src, int line);

#endif /* LEXER_H */
