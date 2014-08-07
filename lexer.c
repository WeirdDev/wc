#include <ctype.h>
#include <string.h>

#include "lexer.h"
#include "error.h"

//#define ISKEYWORD(keyword) (strcmp(currptr, keyword)==0 && !isalpha(currptr[strlen(keyword)]))

plinkedlist lexer_parse(char* source) {
	plinkedlist tokens = ll_new();
	
	int l, currline = 0;
	char* curr, *start;
	while(*curr!='\0') {
		if(*curr=='\n')
			currline++;
		else if(isspace(*curr)) {
			curr++;
			continue;
		}

		else if(isdigit(*curr)) {
			curr = start;
			while(isdigit(*++curr))
				;
			/* to point on the last character of token */
			curr--;
			
			l = (int)(curr - start);
			start = (char*)malloc(l+1);
			strncpy(start, curr-l, l);
			ll_push(tokens, lexer_token_create(TOKEN_NUMBER, start, currline));
		} else if(*curr=='"') {
			start = ++curr; /* skip first doublequote */;
			char slashactive = 0;
			while(*curr!='\0' && !(*curr=='"' && !slashactive)) {
				if(*curr=='\\')
					slashactive = !slashactive;
				else if(slashactive)
					slashactive = 0;
					
				if(*curr=='\n')
					currline++;
				
				curr++;
			}
			/* curr pointing on the last character of token */
			
			l = (int)(curr - start);
			start = (char*)malloc(l+1);
			strncpy(start, curr-l, l);
			ll_push(tokens, lexer_token_create(TOKEN_STRING, start, currline));
		} else if(isalpha(*curr) || *curr=='_') {
			/* nothing found here, it's not a constant token */
			/* could it be an identifier? */
			start = curr;
			do {
				curr++;
			} while(isalnum(*curr) || *curr=='_');
			/* to ensure curr points on characters last token */
			curr--;
				
				
			l = (int)(curr - start);
			start = (char*)malloc(l+1);
			strncpy(start, curr-l, l);
			
			int i;
			for(i = 0;consttokens[i].type != TOKEN_EOF;i++)
				if(*start==*consttokens[i].string)
					/* if the first character equals, check the full string */
					if(strcmp(start, consttokens[i].string) == 0) {
						ll_push(tokens, lexer_token_copy(&consttokens[i], currline));
					}
					/* if they aren't equal, continue */
				/* if the first character isn't equal, continue searching */
			
			if(consttokens[i].type == TOKEN_EOF)
				ll_push(tokens, lexer_token_create(TOKEN_WORD, start, currline));
		} else 
			FATAL("Invalid token following '%c'", *curr);
		
		curr++;
	}
	
	/* mark the EOF */
	ll_push(tokens, lexer_token_create(TOKEN_EOF, NULL, currline));
	
	/* return the linked list with tokens */
	return tokens;
}
pll_entry lexer_parse_brwords(pll_entry tokens, plinkedlist* list) {
	*list = ll_new();
	ptoken t = GETTKN(tokens);

	char even = 0;
	CHECK_TOKEN(t, TOKEN_BRSTART);
	while(t->type != TOKEN_BREND) {
		tokens = NEXTTKN(tokens);
		t = GETTKN(tokens);

		if(!even) {
			CHECK_TOKEN(t, TOKEN_WORD);
			ll_push(*list, t->string);
		} else
			CHECK_TOKEN(t, TOKEN_COMMA);
		even = !even;
	}

	return tokens;
}

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

	{ TOKEN_ASSIGN, "=", 0 },
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

ptoken lexer_token_create(tokentype type, char* string, int line) {
	ptoken ret = (ptoken)malloc(sizeof(token));
	ret->type = type;
	ret->string = string;
	ret->line = line;

	return ret;
}
ptoken lexer_token_copy(ptoken src, int line) {
	ptoken ret = (ptoken)malloc(sizeof(token));
	memcpy(ret, src, sizeof(token));
	ret->line = line;

	return ret;
}
