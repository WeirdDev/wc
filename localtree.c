#include <malloc.h>

#include "error.h"
#include "ll.h"

#include "member.h"
#include "lexer.h"
#include "expressiontree.h"
#include "localtree.h"
#include "treevisitor.h"

accept_method(lsblock) {

}
plsblock syntax_create_lsblock(plinkedlist members) {
	plsblock ret = member_new(lsblock);
	member_set(ret, LSMEMBER_LSBLOCK, &lsblock_accept, 0);
	ret->localspace = members;

	return ret;
}
pll_entry syntax_parse_lsblock(pll_entry tokens, plsblock* localspace) {
	ptoken ftoken = GETTKN(tokens);
	CHECK_TOKEN(ftoken, TOKEN_CBRSTART);
	plinkedlist members = ll_new();

	ptoken t;
	do {
		tokens = NEXTTKN(tokens);
		t = GETTKN(tokens);

		plsmember lsm;
		if(t->base.type==TOKEN_CBRSTART)
			tokens = syntax_parse_lsblock(tokens, (plsblock*)&lsm);
		else
			tokens = syntax_parse_lsmember(tokens, &lsm);

		ll_push(members, lsm);

		tokens = NEXTTKN(tokens);
	} while(t->base.type != TOKEN_CBREND);
	tokens = NEXTTKN(tokens);

	*localspace = syntax_create_lsblock(members);
	(*localspace)->base.base.line = ftoken->base.line;

	return tokens;
}

pll_entry syntax_parse_lsblock_singlemember(pll_entry tokens, plsblock* localspace) {
	plsmember m;

	ptoken ftoken = GETTKN(tokens);
	tokens = syntax_parse_lsmember(tokens, &m);
	plinkedlist ll = ll_new();
	ll_add(ll, m);
	*localspace = syntax_create_lsblock(ll);
	(*localspace)->base.base.line = ftoken->base.line;

	return tokens;
}

pll_entry syntax_parse_lsmember(pll_entry tokens, plsmember* ret) {
	ptoken t = GETTKN(tokens);

	if(t->base.type == TOKEN_VAR)
		tokens = syntax_parse_lsvariable(tokens, (plsvariable*)ret);
	else if(t->base.type == TOKEN_IF)
		tokens = syntax_parse_lsif(tokens, (plsif*)ret);
	else if(t->base.type == TOKEN_WHILE)
		tokens = syntax_parse_lswhile(tokens, (plswhile*)ret);
	else if(t->base.type == TOKEN_RETURN)
		tokens = syntax_parse_lsreturn(tokens, (plsreturn*)ret);
	else {
		tokens = syntax_parse_lsexpression(tokens, (plsexpression*)ret);
		switch(((plsexpression)ret)->expression->base.type) {
			case EXPRESSION_ASSIGN:
			case EXPRESSION_CALL:
			case EXPRESSION_PREFIXINC:
			case EXPRESSION_PREFIXDEC:
			case EXPRESSION_POSTFIXINC:
			case EXPRESSION_POSTFIXDEC:
				/* don't throw an error, these expressions are valid in local space */
				break;
			default:
				FATAL("Invalid expression found in local space on line %d, only assigment, "
						"call, incrementation and decrementation are allowed", (*ret)->base.line);
				break;
		}
	}

	return tokens;
}

accept_method(lsvariable) {

}
pll_entry syntax_parse_lsvariable(pll_entry tokens, plsvariable* ret) {
	ptoken t = GETTKN(tokens);
	tokens = NEXTTKN(tokens);

	plsvariable var = member_new(lsvariable);
	member_sett(var, LSMEMBER_VARIABLE, &lsvariable_accept, t);
	*ret = var;

	CHECK_TOKEN(t, TOKEN_WORD)
	else
		var->name = t->string;

	tokens = NEXTTKN(tokens);
	t = (ptoken)tokens->data;
	if(t->base.type == TOKEN_SEMICOLON)
		return tokens;
	else if(t->base.type != TOKEN_ASSIGN)
		UNEXP_EXP_TOKEN(t, TOKEN_SEMICOLON)
	else {
		tokens = syntax_parse_expression(NEXTTKN(tokens), &var->expression);
		tokens = NEXTTKN(tokens);
		t = GETTKN(tokens);
		CHECK_TOKEN(t, TOKEN_SEMICOLON);

		return tokens;
	}

	return tokens;
}
accept_method(lsexpression) {

}
pll_entry syntax_parse_lsexpression(pll_entry tokens, plsexpression* ret) {
	plsexpression lsexpr = member_new(lsexpression);
	member_set(lsexpr, LSMEMBER_EXPRESSION, &lsexpression_accept, 0);
	*ret = lsexpr;

	tokens = syntax_parse_expression(tokens, &lsexpr->expression);
	lsexpr->base.base.line = lsexpr->expression->base.line;
	tokens = NEXTTKN(tokens);
	CHECK_TOKEN(GETTKN(tokens), TOKEN_SEMICOLON);

	return tokens;
}
accept_method(lswhile) {

}
pll_entry syntax_parse_lswhile(pll_entry tokens, plswhile* ret) {
	ptoken t = GETTKN(tokens);
	tokens = NEXTTKN(tokens);

	plswhile w = member_new(lswhile);
	member_sett(w, LSMEMBER_WHILE, &lswhile_accept, t);
	*ret = w;

	tokens = syntax_parse_exbracket(tokens, (pexbracket*)&w->expression);
	tokens = NEXTTKN(tokens);

	t = GETTKN(tokens);
	if(t->base.type==TOKEN_SEMICOLON)
		return tokens;

	if(t->base.type==TOKEN_CBRSTART)
		return syntax_parse_lsblock(tokens, &w->localspace);

	tokens = syntax_parse_lsblock_singlemember(tokens, &w->localspace);

	return tokens;
}
accept_method(lsif) {

}
pll_entry syntax_parse_lsif(pll_entry tokens, plsif* ret) {
	ptoken t = GETTKN(tokens);
	tokens = NEXTTKN(tokens);

	plsif i = member_new(lsif);
	member_sett(i, LSMEMBER_IF, &lsif_accept, t);
	*ret = i;

	tokens = syntax_parse_exbracket(tokens, (pexbracket*)&i->expression);
	tokens = NEXTTKN(tokens);

	t = GETTKN(tokens);
	if(t->base.type==TOKEN_CBRSTART)
		tokens = syntax_parse_lsblock(tokens, &i->localspace);
	else if(t->base.type==TOKEN_SEMICOLON) {
		/* intentionally empty */
	} else
		tokens = syntax_parse_lsblock_singlemember(tokens, &i->localspace);

	tokens = NEXTTKN(tokens);
	t = GETTKN(tokens);
	if(t->base.type==TOKEN_ELSE)
		tokens = syntax_parse_lselse(tokens, &i->elseblock);
	else
		tokens = tokens->prev; /* go back with tokens, it must be pointing on last element processed by this function */

	return tokens;

}
accept_method(lselse) {

}
pll_entry syntax_parse_lselse(pll_entry tokens, plselse* ret) {
	ptoken t = GETTKN(tokens);
	tokens = NEXTTKN(tokens);

	plselse lsels = member_new(lselse);
	member_sett(lsels, LSMEMBER_ELSE, &lselse_accept, t);
	*ret = lsels;

	t = GETTKN(tokens);
	if(t->base.type==TOKEN_CBRSTART)
		return syntax_parse_lsblock(tokens, &lsels->localspace);

	tokens = syntax_parse_lsblock_singlemember(tokens, &lsels->localspace);

	return tokens;
}
accept_method(lsreturn) {

}
pll_entry syntax_parse_lsreturn(pll_entry tokens, plsreturn* ret) {
	ptoken t = GETTKN(tokens);
	tokens = NEXTTKN(tokens);

	plsreturn i = member_new(lsreturn);
	member_sett(i, LSMEMBER_RETURN, &lsreturn_accept, t);
	*ret = i;

	t = GETTKN(tokens);
	if(t->base.type==TOKEN_SEMICOLON)
		i->expression = NULL;
	else
		tokens = syntax_parse_expression(tokens, &i->expression);

	return tokens;
}
