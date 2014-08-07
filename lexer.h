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
#define TOKEN_INCREMENT		60
#define TOKEN_DECREMENT		61
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

token consttokens[] = { 
	{ TOKEN_VAR, "var", 0 },
	{ TOKEN_FUNCTION, "function", 0 },
	{ TOKEN_IF, "if", 0 },
	{ TOKEN_ELSE, "else", 0 },
	{ TOKEN_WHILE, "while", 0 },
	
	{ TOKEN_PLUS, "+", 0 },
	{ TOKEN_MINUS, "-", 0 },
	{ TOKEN_MUL, "*", 0 },
	{ TOKEN_DIV, "/", 0 },
	{ TOKEN_MOD, "%", 0 },
	
	{ TOKEN_EQUAL, "==", 0 },
	{ TOKEN_NOTEQUAL, "!=", 0 },
	{ TOKEN_SMALLERTHAN, "<", 0 },
	{ TOKEN_BIGGERTHAN, ">", 0 },
	{ TOKEN_SMALLEREQUAL, "<=", 0 },
	{ TOKEN_BIGGEREQUAL, ">=", 0 },
	{ TOKEN_NEGATION, "!", 0 },
	
	{ TOKEN_INCREMENT, "++", 0 },
	{ TOKEN_DECREMENT, "--", 0 },
	
	{ TOKEN_COLON, ":", 0 },
	{ TOKEN_SEMICOLON, ";", 0 },
	{ TOKEN_COMMA, ",", 0 },
	{ TOKEN_CBRSTART, "{", 0 },
	{ TOKEN_CBREND, "}", 0 },
	{ TOKEN_BRSTART, "(", 0 },
	{ TOKEN_BREND, ")", 0 },
	{ TOKEN_IBRSTART, "[", 0 },
	{ TOKEN_IBREND, "]", 0 }, 
	
	{ TOKEN_EOF, NULL, 0 }
}; 

#include "ll.h"
#include <malloc.h>
extern plinkedlist lexer_parse(char* source, unsigned int* tokensn);

inline ptoken lexer_token_create(tokentype type, char* string, int line) {
	ptoken ret = (ptoken)malloc(sizeof(token));
	ret->type = type;
	ret->string = string;
	ret->line = line;
	
	return ret;
}
inline ptoken lexer_token_copy(ptoken src, int line) {
	ptoken ret = (ptoken)malloc(sizeof(token));
	memcpy(ret, src, sizeof(token));
	ret->line = line;
	
	return ret;
}

#endif /* LEXER_H */