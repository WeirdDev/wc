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
#define TOKEN_SEMICOLON		70
#define TOKEN_CBRSTART		71
#define TOKEN_CBREND		72
#define TOKEN_BRSTART		73
#define TOKEN_BREND			74
#define TOKEN_IBRSTART		75
#define TOKEN_IBREND		76

typedef struct token_t {
	tokentype type;
	char* string;
} token, *ptoken;

token consttokens[] = { 
	{ TOKEN_VAR, "var" },
	{ TOKEN_FUNCTION, "function" },
	{ TOKEN_IF, "if" },
	{ TOKEN_ELSE, "else" },
	{ TOKEN_WHILE, "while" },
	
	{ TOKEN_PLUS, "+" },
	{ TOKEN_MINUS, "-" },
	{ TOKEN_MUL, "*" },
	{ TOKEN_DIV, "/" },
	{ TOKEN_MOD, "%" },
	
	{ TOKEN_EQUAL, "==" },
	{ TOKEN_NOTEQUAL, "!=" },
	{ TOKEN_SMALLERTHAN, "<" },
	{ TOKEN_BIGGERTHAN, ">" },
	{ TOKEN_SMALLEREQUAL, "<=" },
	{ TOKEN_BIGGEREQUAL, ">=" },
	{ TOKEN_NEGATION, "!" },
	
	{ TOKEN_INCREMENT, "++" },
	{ TOKEN_DECREMENT, "--" },
	
	{ TOKEN_SEMICOLON, ";" },
	{ TOKEN_CBRSTART, "{" },
	{ TOKEN_CBREND, "}" },
	{ TOKEN_BRSTART, "(" },
	{ TOKEN_BREND, ")" },
	{ TOKEN_IBRSTART, "[" },
	{ TOKEN_IBREND, "]" }, 
	
	{ TOKEN_EOF, NULL }
}; 

#include "ll.h"
#include <malloc.h>
extern plinkedlist lexer_parse(char* source, unsigned int* tokensn);

inline ptoken lexer_token_create(tokentype type, char* string) {
	ptoken ret = (ptoken)malloc(sizeof(token));
	ret->type = type;
	ret->string = string;
	
	return ret;
}

#endif /* LEXER_H */