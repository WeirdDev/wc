#include <ctype.h>
#include <string.h>

#include "lexer.h"
#include "error.h"
#include "treevisitor.h"

//#define ISKEYWORD(keyword) (strcmp(currptr, keyword)==0 && !isalpha(currptr[strlen(keyword)]))

plinkedlist lexer_parse(char* source) {
	plinkedlist tokens = ll_new();

	char inSlComment = 0, inMlComment = 0;
	int l, currline = 1;
	char* curr, *start;
	while(*curr!='\0') {
		if(*curr=='\n') {
			currline++;
			if(inSlComment) //single
				inSlComment = !inSlComment;
		} else if(isspace(*curr)) {
			/* curr++;
			continue; */ //<-- same as leaving execution to go on (jumping through ifelses has the same effect)
		} else if(*curr=='/') {
			if(!inSlComment && !inMlComment) {
				if(curr[1]=='/')
					inSlComment = !inSlComment;
				else if(curr[1]=='*')
					inMlComment = !inMlComment;
			}
		} else if(*curr=='*' && !inSlComment && inMlComment) {
			if(curr[1]=='/')
				inMlComment = !inMlComment;
		} else if(inSlComment || inMlComment) {
			/* curr++;
			continue; */ //<-- same as leaving execution to go on (jumping through ifelses has the same effect)
		}

		else if(isdigit(*curr)) {
			start = curr;
			while(isdigit(*++curr))
				;
			/* to point on the last character of token */
			curr--;
			
			l = (int)(curr - start+1);
			start = (char*)malloc(l+1);
			strncpy(start, curr-l+1, l);
			start[l] = '\0';

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
			
			l = (int)(curr - start +1);
			start = (char*)malloc(l+1);
			strncpy(start, curr-l+1, l);
			start[l] = '\0';

			ll_push(tokens, lexer_token_create(TOKEN_STRING, start, currline));
		} else if(isalpha(*curr) || *curr=='_') {
			/* nothing found here, it's not a constant token */
			/* could it be an identifier? */
			start = curr;
			do {
				curr++;
			} while(isalnum(*curr) || *curr=='_');
				
			l = (int)(curr - start);
			start = (char*)malloc(l+1);
			strncpy(start, curr-l, l);
			start[l] = '\0';
			
			/* to ensure curr points on characters last token */
			curr--;

			int i;
			for(i = 0;keywords[i].base.type != TOKEN_EOF;i++)
				if(*start==*keywords[i].string)
					/* if the first character equals, check the full string */
					if(strcmp(start, keywords[i].string) == 0) {
						ll_push(tokens, lexer_token_copy(&keywords[i], currline));
						/* (we don't need the local copy of the string anymore) */
						free(start);

						break;
					}
					/* if they aren't equal, continue */
				/* if the first character isn't equal, continue searching */
			
			/* if this is not a keyword, it's a word (indentfier) */
			if(keywords[i].base.type == TOKEN_EOF)
				ll_push(tokens, lexer_token_create(TOKEN_WORD, start, currline));
		} else {
			int i, occ, occi = 0, c = 0;
			do {
				occ = 0;
				for(i = 0;operators[i].base.type != TOKEN_EOF;i++)
					if((operators[i].string)[c]==curr[c]) {
						occ++;
						occi = i;
					}

				c++;
			} while(occ>1);

			if(occ==0 && c==1)
				FATAL("Invalid token '%c' found on line %d", *curr, currline)
			else
				ll_push(tokens, lexer_token_copy(&operators[occi], currline));
		}
		
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

	tokens = NEXTTKN(tokens);
	t = GETTKN(tokens);
	while(t->base.type != TOKEN_BREND) {
		if(!even) {
			CHECK_TOKEN(t, TOKEN_WORD);
			ll_push(*list, t->string);
		} else
			CHECK_TOKEN(t, TOKEN_COMMA);
		even = !even;

		tokens = NEXTTKN(tokens);
		t = GETTKN(tokens);
	}

	return tokens;
}

token keywords[] = {
	{ { TOKEN_VAR, NULL },		"var",	0 },
	{ { TOKEN_FUNCTION, NULL },	"function",	0 },
	{ { TOKEN_IF, NULL },		"if",	0 },
	{ { TOKEN_ELSE, NULL },		"else",	0 },
	{ { TOKEN_WHILE, NULL },	"while",0 },
	{ { TOKEN_RETURN, NULL },	"return",	0 },

	{ { TOKEN_NULL, NULL },		"null",	0},

	{ { TOKEN_EOF, NULL}, 		NULL,	0 }
};
token operators[] = {
		/* shorter versions of operators must always be after the longer in this array
		 * this means for example + must be after ++, & must be after && ... */
		{ { TOKEN_INCREMENT, NULL },	"++",	0 },
		{ { TOKEN_DECREMENT, NULL },	"--",	0 },

		{ { TOKEN_PLUS, NULL },		"+",	0 },
		{ { TOKEN_MINUS, NULL },	"-",	0 },
		{ { TOKEN_MUL, NULL },		"*",	0 },
		{ { TOKEN_DIV, NULL },		"/",	0 },
		{ { TOKEN_MOD, NULL },		"%",	0 },

		{ { TOKEN_SHL, NULL },	"<<",	0 },
		{ { TOKEN_SHR, NULL },	">>",	0 },

		{ { TOKEN_EQUAL, NULL },	"==",	0 },
		{ { TOKEN_NOTEQUAL, NULL }, "!=",	0 },
		{ { TOKEN_SMALLERTHAN, NULL }, 	"<",	0 },
		{ { TOKEN_BIGGERTHAN, NULL },	">",	0 },
		{ { TOKEN_SMALLEREQUAL, NULL },	"<=",	0 },
		{ { TOKEN_BIGGEREQUAL, NULL },	">=",	0 },
		{ { TOKEN_NEGATION, NULL },		"!",	0 },
		{ { TOKEN_LOGICAND, NULL },	"&&",	0 },
		{ { TOKEN_LOGICOR, NULL },	"||",	0 },

		{ { TOKEN_AND, NULL },	"&",	0 },
		{ { TOKEN_OR, NULL },	"|",	0 },
		{ { TOKEN_XOR, NULL },	"^",	0 },
		{ { TOKEN_NOT, NULL },	"~",	0 },

		{ { TOKEN_ASSIGN, NULL },	"=",	0 },

		{ { TOKEN_COLON, NULL },	":",	0 },
		{ { TOKEN_SEMICOLON, NULL },";",	0 },
		{ { TOKEN_COMMA, NULL },	",",	0 },
		{ { TOKEN_CBRSTART, NULL },	"{",	0 },
		{ { TOKEN_CBREND, NULL },	"}",	0 },
		{ { TOKEN_BRSTART, NULL },	"(",	0 },
		{ { TOKEN_BREND, NULL },	")",	0 },
		{ { TOKEN_IBRSTART, NULL },	"[",	0 },
		{ { TOKEN_IBREND, NULL },	"]",	0 },

		{ { TOKEN_EOF, NULL}, 		NULL,	0 }
};

accept_method(token) {

}
ptoken lexer_token_create(membertype type, char* string, int line) {
	ptoken ret = (ptoken)malloc(sizeof(token));
	ret->base.type = type;
	ret->base.accept = &token_accept;
	ret->string = string;
	ret->line = line;

	return ret;
}
ptoken lexer_token_copy(ptoken src, int line) {
	ptoken ret = (ptoken)malloc(sizeof(token));
	memcpy(ret, src, sizeof(token));
	ret->base.accept = &token_accept;
	ret->line = line;

	return ret;
}
