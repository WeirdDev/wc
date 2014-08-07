#ifndef LEXER_H
#define LEXER_H

typedef char tokentype;
/* EOF */
#define TOKEN_EOF			0
/* Identifier */
#define TOKEN_WORD			10
/* Keywords */
#define TOKEN_VAR			20
#define TOKEN_FUNCTION		21
#define TOKEN_IF			22
#define TOKEN_ELSE			23
#define TOKEN_WHILE			24
/* Constants */
#define TOKEN_NUMBER		30
#define TOKEN_STRING		31
/* Arithmetic operators */
#define TOKEN_PLUS			40
#define TOKEN_MINUS			41
#define TOKEN_MUL			42
#define TOKEN_DIV			43
#define TOKEN_MOD			44
/* Logic operators */
#define TOKEN_EQUAL			50
#define TOKEN_NOTEQUAL		51
#define TOKEN_SMALLERTHAN	52
#define TOKEN_BIGGERTHAN	53
#define TOKEN_SMALLEREQUAL	54
#define TOKEN_BIGGEREQUAL	55
#define TOKEN_NEGATION		56
/* Other operators */
#define TOKEN_ASSIGN		60
#define TOKEN_INCREMENT		61
#define TOKEN_DECREMENT		62
/* Misc */
#define TOKEN_COLON			70
#define TOKEN_SEMICOLON		71
#define TOKEN_COMMA			72
#define TOKEN_CBRSTART		73
#define TOKEN_CBREND		74
#define TOKEN_BRSTART		75
#define TOKEN_BREND			76
#define TOKEN_IBRSTART		77
#define TOKEN_IBREND		78

typedef struct token_t {
	tokentype type;
	char* string;
	
	int line;
} token, *ptoken;

extern token consttokens[];

#include "ll.h"
#include <malloc.h>
extern plinkedlist lexer_parse(char* source);
extern pll_entry lexer_parse_brwords(pll_entry tokens, plinkedlist* list);

#define GETTKN(tokens)	((ptoken)tokens->data)
#define NEXTTKN(tokens)	(tokens->next)

#define UNEXP_TOKEN(ptoken)					{ FATAL("Unexpected token on line %d: '%s'", ptoken->line, ptoken->string); }
#define UNEXP_EXP_TOKEN(ptoken, expected)	{ FATAL("Unexpected token '%s' on line %d, expected '%s'", ptoken->string, ptoken->line, #expected ); }

#define CHECK_TOKEN(ptoken, exptype)		if((ptoken)->type != exptype) { UNEXP_EXP_TOKEN(ptoken, exptype); }

extern ptoken lexer_token_create(tokentype type, char* string, int line);
extern ptoken lexer_token_copy(ptoken src, int line);

#endif /* LEXER_H */
