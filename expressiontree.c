#include <malloc.h>

#include "error.h"
#include "lexer.h"
#include "expressiontree.h"

pll_entry syntax_parse_expression(pll_entry tokens, pexpression* expression) {
	//TODO: work on expressions
	return tokens;
}

/*pll_entry syntax_parse_exterminal(pll_entry tokens, pexpression* ret) {
	tokens = NEXTTKN(tokens);
	ptoken p = GETTKN(tokens);

	if(p->base.type == TOKEN_BRSTART) {

	} else if(p->base.type == TOKEN_WORD) {
		tokens = NEXTTKN(tokens);
		p = GETTKN(tokens);
		tokens = tokens->prev->prev;
		if(p->base.type == TOKEN_BRSTART) {
			tokens = syntax_parse_excall(tokens, ret);
		} else if(p->base.type == TOKEN_IBRSTART) {
			tokens = syntax_parse_exindex(tokens, ret);
		} else if(p->base.type == TOKEN_INCREMENT || (p->base.type == TOKEN_DECREMENT) {
			*ret = (pexpression)malloc(sizeof(exunary));
			((pexunary)*ret)->expression =
		}
	}
	return tokens;
}*/

pll_entry syntax_parse_exprefixunary(pll_entry tokens, pexunary* ret) {
	//TODO: parse prefixunary
	return tokens;
}

pll_entry syntax_parse_exbracket(pll_entry tokens, pbracketexpression* expression) {
	//TODO: work on this kind of expressions
	return tokens;
}
