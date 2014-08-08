#include <malloc.h>

#include "error.h"
#include "ll.h"

#include "lexer.h"
#include "expressiontree.h"
#include "localtree.h"
#include "treevisitor.h"

accept_method(lsblock) {

}
plsblock syntax_create_lsblock(plinkedlist members) {
	plsblock ret = (plsblock)malloc(sizeof(lsblock));
	ret->base.base.type = LSMEMBER_LSBLOCK;
	ret->base.base.accept = &lsblock_accept;
	ret->localspace = members;

	return ret;
}
pll_entry syntax_parse_lsblock(pll_entry tokens, plsblock* localspace) {
	ptoken t = GETTKN(tokens);
	tokens = NEXTTKN(tokens);
	CHECK_TOKEN(t, TOKEN_CBRSTART);
	plinkedlist members = ll_new();

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

	return tokens;
}

pll_entry syntax_parse_lsblock_singlemember(pll_entry tokens, plsblock* localspace) {
	plsmember m;
	tokens = syntax_parse_lsmember(tokens, &m);
	plinkedlist ll = ll_new();
	ll_add(ll, m);
	*localspace = syntax_create_lsblock(ll);

	return tokens;
}

pll_entry syntax_parse_lsmember(pll_entry tokens, plsmember* ret) {
	ptoken t = GETTKN(tokens);
	tokens = NEXTTKN(tokens);

	if(t->base.type == TOKEN_VAR)
		tokens = syntax_parse_lsvariable(tokens, (plsvariable*)ret);
	else if(t->base.type == TOKEN_IF)
		tokens = syntax_parse_lsif(tokens, (plsif*)ret);
	else if(t->base.type == TOKEN_WHILE)
		tokens = syntax_parse_lswhile(tokens, (plswhile*)ret);
	else {
		tokens = syntax_parse_lsexpression(tokens, (plsexpression*)ret);
		switch(((plsexpression)ret)->expression->base.type) {
			case EXPRESSION_ASSIGN:
			case EXPRESSION_CALL:
			case EXPRESSION_INCREMENT:
			case EXPRESSION_DECREMENT:
				/* don't throw an error, this is valid */
				break;
			default:
				FATAL("Invalid expression found in local space, only assigment, "
						"call, incrementation and decrementation are allowed");
				break;
		}
	}

	return tokens;
}

accept_method(lsvariable) {

}
pll_entry syntax_parse_lsvariable(pll_entry tokens, plsvariable* ret) {
	plsvariable var = (plsvariable)malloc(sizeof(lsvariable));
	var->base.base.type = LSMEMBER_VARIABLE;
	var->base.base.accept = &lsvariable_accept;
	*ret = var;

	ptoken t = GETTKN(tokens);
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
	plsexpression lsexpr = (plsexpression)malloc(sizeof(lsexpression));
	lsexpr->base.base.type = LSMEMBER_EXPRESSION;
	lsexpr->base.base.accept = &lsexpression_accept;
	*ret = lsexpr;

	tokens = syntax_parse_expression(tokens, &lsexpr->expression);
	tokens = NEXTTKN(tokens);
	CHECK_TOKEN(GETTKN(tokens), TOKEN_SEMICOLON);

	return tokens;
}
accept_method(lswhile) {

}
pll_entry syntax_parse_lswhile(pll_entry tokens, plswhile* ret) {
	plswhile w = (plswhile)malloc(sizeof(lswhile));
	w->base.base.type = LSMEMBER_WHILE;
	w->base.base.accept = &lswhile_accept;
	*ret = w;

	tokens = syntax_parse_bracketexpression(tokens, (pbracketexpression*)&w->expression);
	tokens = NEXTTKN(tokens);

	ptoken t = GETTKN(tokens);
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
	plsif i = (plsif)malloc(sizeof(lsif));
	i->base.base.type = LSMEMBER_IF;
	i->base.base.accept = &lsif_accept;
	*ret = i;

	tokens = syntax_parse_bracketexpression(tokens, (pbracketexpression*)&i->expression);
	tokens = NEXTTKN(tokens);

	ptoken t = GETTKN(tokens);
	if(t->base.type==TOKEN_CBRSTART)
		tokens = syntax_parse_lsblock(tokens, &i->localspace);
	else if(t->base.type==TOKEN_SEMICOLON) {
		/* intentionally empty */
	} else
		tokens = syntax_parse_lsblock_singlemember(tokens, &i->localspace);

	tokens = NEXTTKN(tokens);
	ptoken p = GETTKN(tokens);
	if(t->base.type==TOKEN_ELSE)
		tokens = syntax_parse_lselse(tokens, &i->elseblock);
	else
		tokens = tokens->prev; /* go back with tokens, it must be pointing on last element processed by this function */

	return tokens;

}
pll_entry syntax_parse_lselse(pll_entry tokens, plselse* ret) {
	plselse lsels = (plselse)malloc(sizeof(lselse));
	lsels->base.base.type = LSMEMBER_ELSE;
	*ret = lsels;

	ptoken t = GETTKN(tokens);
	if(t->base.type==TOKEN_CBRSTART)
		return syntax_parse_lsblock(tokens, &lsels->localspace);

	tokens = syntax_parse_lsblock_singlemember(tokens, &lsels->localspace);

	return tokens;
}
