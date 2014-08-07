#include <ctype.h>
#include <string.h>

#include "lexer.h"

//#define ISKEYWORD(keyword) (strcmp(currptr, keyword)==0 && !isalpha(currptr[strlen(keyword)]))

plinkedlist lexer_parse(char* source, unsigned int* tokensn) {
	plinkedlist tokens = ll_new();
	
	int l;
	char* curr, *start;
	while(*curr!='\0') {
		if(isdigit(*curr)) {
			curr = start;
			while(isdigit(*++curr))
				;
			/* to point on the last character of token */
			curr--;
			
			l = (int)(curr - start);
			start = (char*)malloc(l+1);
			strncpy(start, curr-l, l);
			ll_push(tokens, lexer_token_create(TOKEN_NUMBER, start));
		} else if(*curr=='"') {
			start = ++curr; /* skip first doublequote */;
			char slashactive = 0;
			while(*curr!='\0' && !(*curr=='"' && !slashactive)) {
				if(*curr=='\\')
					slashactive = !slashactive;
				else if(slashactive)
					slashactive = 0;
				
				curr++;
			}
			/* curr pointing on the last character of token */
			
			l = (int)(curr - start);
			start = (char*)malloc(l+1);
			strncpy(start, curr-l, l);
			ll_push(tokens, lexer_token_create(TOKEN_STRING, start));
		} else {
			/* nothing found here, it's not a constant token */
			/* could it be an identifier? */
			if(isalpha(*curr) || *curr=='_') {
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
							ll_push(tokens, &consttokens[i]);
						}
						/* if they aren't equal, continue */
					/* if the first character isn't equal, continue searching */
				
				if(consttokens[i].type == TOKEN_EOF)
					ll_push(tokens, lexer_token_create(TOKEN_WORD, start));
			}
		}
		
		curr++;
	}
	
	/* mark the EOF */
	ll_push(tokens, lexer_token_create(TOKEN_EOF, NULL));
	
	/* return the linked list with tokens */
	return tokens;
}