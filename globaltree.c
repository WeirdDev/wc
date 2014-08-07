#include <malloc.h>

#include "error.h"

#include "lexer.h"
#include "globaltree.h"

pll_entry syntax_parse_gsmember(pll_entry tokens, pgsmember* ret) {
	ptoken t = GETTKN(tokens);
	if(t->type == TOKEN_VAR)
		return syntax_parse_gsvariable(NEXTTKN(tokens), (pgsvariable*)ret);
	else if(t->type == TOKEN_FUNCTION)
		return syntax_parse_gsfunction(NEXTTKN(tokens), (pgsfunction*)ret);
	else {
		UNEXP_TOKEN(t);
		return tokens;
	}

}
pll_entry syntax_parse_gsvariable(pll_entry tokens, pgsvariable* ret) {
	pgsvariable var = (pgsvariable)malloc(sizeof(gsvariable));
	var->gsmember.type = GSMEMBER_VARIABLE;
	*ret = var;

	ptoken t = GETTKN(tokens);
	CHECK_TOKEN(t, TOKEN_WORD)
	else
		var->name = t->string;

	tokens = NEXTTKN(tokens);
	t = (ptoken)tokens->data;
	if(t->type == TOKEN_SEMICOLON)
		return tokens;
	else if(t->type != TOKEN_ASSIGN)
		UNEXP_EXP_TOKEN(t, TOKEN_SEMICOLON)
	else
		return syntax_parse_expression(NEXTTKN(tokens), &var->expression);

	return tokens;
}
pll_entry syntax_parse_gsfunction(pll_entry tokens, pgsfunction* ret) {
	pgsfunction fn = (pgsfunction)malloc(sizeof(gsfunction));
	fn->gsmember.type = GSMEMBER_FUNCTION;
	*ret = fn;

	ptoken t = (ptoken)tokens->data;
	CHECK_TOKEN(t, TOKEN_WORD)
	else
		fn->name = t->string;

	tokens = lexer_parse_brwords(tokens, &fn->parameters);
	tokens = NEXTTKN(tokens);
	t = GETTKN(tokens);

	if(t->type==TOKEN_SEMICOLON)
		return tokens;

	tokens = syntax_parse_lsblock(tokens, &fn->localspace);
	tokens = NEXTTKN(tokens);
	t = GETTKN(tokens);

	return tokens;
}