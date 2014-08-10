#include <stdlib.h>
#include <malloc.h>

#include "member.h"
#include "error.h"
#include "lexer.h"
#include "expressiontree.h"
#include "treevisitor.h"

#define ASSOC_LEFT	1
#define ASSOC_RIGHT	0
membertype binaryexpressions[][4] = {
	{ TOKEN_PLUS,	EXPRESSION_ADD, 10, ASSOC_LEFT },
	{ TOKEN_MINUS,	EXPRESSION_SUB, 10, ASSOC_LEFT },
	{ TOKEN_MUL,	EXPRESSION_MUL, 11, ASSOC_LEFT },
	{ TOKEN_DIV,	EXPRESSION_DIV, 11, ASSOC_LEFT },
	{ TOKEN_MOD,	EXPRESSION_MOD,	11, ASSOC_LEFT },
	{ TOKEN_AND,	EXPRESSION_AND,	6, ASSOC_LEFT },
	{ TOKEN_OR,		EXPRESSION_OR,	4, ASSOC_LEFT },
	{ TOKEN_XOR,	EXPRESSION_XOR, 5, ASSOC_LEFT },
	{ TOKEN_SHL,	EXPRESSION_SHL, 9, ASSOC_LEFT },
	{ TOKEN_SHR,	EXPRESSION_SHR,	9, ASSOC_LEFT },

	{ TOKEN_EQUAL,	EXPRESSION_EQUAL,	7, ASSOC_LEFT },
	{ TOKEN_NOTEQUAL,		EXPRESSION_NOTEQUAL,	7, ASSOC_LEFT },
	{ TOKEN_SMALLERTHAN,	EXPRESSION_SMALLERTHAN,	8, ASSOC_LEFT },
	{ TOKEN_BIGGERTHAN,		EXPRESSION_BIGGERTHAN,	8, ASSOC_LEFT },
	{ TOKEN_SMALLEREQUAL,	EXPRESSION_SMALLEREQUAL,8, ASSOC_LEFT },
	{ TOKEN_BIGGEREQUAL,	EXPRESSION_BIGGEREQUAL,	8, ASSOC_LEFT },
	{ TOKEN_LOGICOR,	EXPRESSION_LOGICOR,	2, ASSOC_LEFT },
	{ TOKEN_LOGICAND,	EXPRESSION_LOGICAND,3, ASSOC_LEFT },

	{ TOKEN_ASSIGN,			EXPRESSION_ASSIGN,	1, ASSOC_RIGHT },
	{ TOKEN_ASSIGNPLUS,		EXPRESSION_ASSIGNPLUS,	1, ASSOC_RIGHT },
	{ TOKEN_ASSIGNMINUS,	EXPRESSION_ASSIGNMINUS,	1, ASSOC_RIGHT },
	{ TOKEN_ASSIGNMUL,		EXPRESSION_ASSIGNMUL,	1, ASSOC_RIGHT },
	{ TOKEN_ASSIGNDIV,		EXPRESSION_ASSIGNDIV,	1, ASSOC_RIGHT },
	{ TOKEN_ASSIGNMOD,		EXPRESSION_ASSIGNMOD,	1, ASSOC_RIGHT },
	{ TOKEN_ASSIGNAND,		EXPRESSION_ASSIGNAND,	1, ASSOC_RIGHT },
	{ TOKEN_ASSIGNOR,		EXPRESSION_ASSIGNOR,	1, ASSOC_RIGHT },
	{ TOKEN_ASSIGNXOR,		EXPRESSION_ASSIGNXOR,	1, ASSOC_RIGHT },
	{ TOKEN_ASSIGNSHL,		EXPRESSION_ASSIGNSHL,	1, ASSOC_RIGHT },
	{ TOKEN_ASSIGNSHR,		EXPRESSION_ASSIGNSHR,	1, ASSOC_RIGHT },

	{ TOKEN_EOL,		0,	0, ASSOC_LEFT }
};
membertype browsebinaryexpressions(ptoken t, int forWhat) {
	int i;
	for(i = 0;binaryexpressions[i][0] != TOKEN_EOL;i++)
		if(binaryexpressions[i][0] == t->base.type)
			return binaryexpressions[i][forWhat];

	return -1;
}
#define getexbntype(t)	browsebinaryexpressions(t, 1)
#define getprec(t)		browsebinaryexpressions(t, 2)
#define getassoc(t)		browsebinaryexpressions(t, 3)
pll_entry syntax_parse_expression_p(pll_entry tokens, pexpression* ret, int minprec) {
	pexpression result;
	tokens = syntax_parse_exterminal(tokens, &result);

	tokens = NEXTTKN(tokens);
	ptoken t = GETTKN(tokens);

	int nextprec; ptoken currentoperator;
	while((nextprec = getprec(t)) >= minprec) {
		currentoperator = t;
		switch(t->base.type) {
			case TOKEN_SEMICOLON:
			case TOKEN_COLON:
			case TOKEN_COMMA:
			case TOKEN_BREND:
			case TOKEN_CBREND:
				goto returnpoint;
				break;
		}
		int associativity = getassoc(currentoperator);
		if(associativity == ASSOC_LEFT)
			nextprec++;

		pexpression right;
		tokens = NEXTTKN(tokens);
		tokens = syntax_parse_expression_p(tokens, &right, nextprec);
		result = (pexpression)syntax_create_exbinary(currentoperator, result, right);

		tokens = NEXTTKN(tokens);
		t = GETTKN(tokens);
	}

returnpoint:
	*ret = result;
	return tokens->prev;
}

pll_entry syntax_parse_expression(pll_entry tokens, pexpression* expression) {
	return syntax_parse_expression_p(tokens, expression, 0 /* starting minimal precedence is 0 */);
}
pll_entry syntax_parse_expressionlist(pll_entry tokens, plinkedlist* expressions) {
	ptoken ftoken = GETTKN(tokens);
	plinkedlist list = ll_new();

	tokens = NEXTTKN(tokens);
	ptoken t = GETTKN(tokens);
	while(t->base.type != TOKEN_BREND && t->base.type != TOKEN_CBREND) {
		pexpression expr;
		tokens = syntax_parse_expression(tokens, &expr);
		ll_push(list, expr);

		tokens = NEXTTKN(tokens);
		t = GETTKN(tokens);
		if(t->base.type == TOKEN_COMMA) {
			tokens = NEXTTKN(tokens);
			t = GETTKN(tokens);
		}
	}

	if(ftoken->base.type == TOKEN_BRSTART && t->base.type != TOKEN_BREND)
		FATAL("Unexpected token '%s' on line %d, expected ')'", t->string, t->base.line)
	else if(ftoken->base.type == TOKEN_CBRSTART && t->base.type != TOKEN_CBREND)
		FATAL("Unexpected token '%s' on line %d, expected '}'", t->string, t->base.line)

	return tokens;
}

accept_method(exconstant);
accept_method(exvariable);
accept_method(excall);
accept_method(exunary);
accept_method(exbinary);
accept_method(externary);
accept_method(exbracket);
accept_method(exindex);
accept_method(exbracket);
accept_method(exarray);

accept_method(exvariable) {

}
pll_entry syntax_parse_exterminal(pll_entry tokens, pexpression* ret) {
	pll_entry ftoken = tokens;	//the first token
	ptoken p = GETTKN(tokens);

	if(p->base.type == TOKEN_BRSTART) {
		tokens = syntax_parse_exbracket(ftoken, (pexbracket*)ret);
	} else if(p->base.type == TOKEN_CBRSTART) {
		tokens = syntax_parse_exarray(ftoken, (pexarray*)ret);
	} else if(p->base.type == TOKEN_WORD) {
		tokens = NEXTTKN(tokens);
		p = GETTKN(tokens);
		if(p->base.type == TOKEN_BRSTART) {
			tokens = syntax_parse_excall(ftoken, (pexcall*)ret);
		} else {
			p = GETTKN(ftoken);
			*ret = (pexpression)member_new(exvariable);
			member_sett(*ret, EXPRESSION_VARIABLE, &exvariable_accept, p);
			((pexvariable)*ret)->name = p->string;

			/* go back a token */
			tokens = tokens->prev;
		}
	} else if(p->base.type == TOKEN_NUMBER || p->base.type == TOKEN_STRING || p->base.type == TOKEN_NULL) {
		tokens = syntax_parse_exconstant(ftoken, (pexconstant*)ret);
	} else {
		tokens = syntax_parse_exprefixunary(ftoken, (pexunary*)ret);
	}

	/* go one token ahead, so we can find out if there isn't any postfix or ternary operator applied */
	tokens = NEXTTKN(tokens);
	p = GETTKN(tokens);
	if(p->base.type == TOKEN_INCREMENT || p->base.type == TOKEN_INCREMENT) {
		switch((*ret)->base.type) {
		case EXPRESSION_VARIABLE:
		case EXPRESSION_INDEX:
			/* these are valid */
			break;
		default:
			FATAL("Invalid use of postfix operator '%s' on line %d", p->string, p->base.line);
			break;
		}

		pexunary tmp = member_new(exunary);
		member_sett(tmp, (p->base.type == TOKEN_INCREMENT ? EXPRESSION_PREFIXINC : EXPRESSION_PREFIXDEC), &exunary_accept, p);
		tmp->expression = *ret;

		*ret = (pexpression)tmp;
	} else if(p->base.type == TOKEN_IBRSTART) {
		pexpression tmp = *ret;
		switch(tmp->base.type) {
			case EXPRESSION_VARIABLE:
			case EXPRESSION_CALL:
			case EXPRESSION_ARRAY:
			case EXPRESSION_INDEX:
				/* these are valid */
				break;
			default:
				FATAL("Invalid use of indexing operator on line %d (expression not indexable)", p->base.line);
				break;
		}

		tokens = syntax_parse_exindex(tokens, (pexindex*)ret);
		((pexindex)*ret)->indexable = tmp;
	} else if(p->base.type == TOKEN_QMARK) {
		pexpression tmp = *ret;

		tokens = NEXTTKN(tokens);
		tokens = syntax_parse_externary(tokens, (pexternary*)ret);
		((pexternary)*ret)->cond = tmp;
	}

	/* go back on the last token of just captured terminal */
	tokens = tokens->prev;
	return tokens;
}

accept_method(exconstant) {

}
pll_entry syntax_parse_exconstant(pll_entry tokens, pexconstant* ret) {
	ptoken t = GETTKN(tokens);
	pexconstant c = member_new(exconstant);
	member_sett(c, 0, &exconstant_accept, t);
	*ret = c;

	switch(t->base.type) {
		case TOKEN_NUMBER:
			c->value.number = atoi(t->string);
			c->base.base.type = EXPRESSION_NUMBER;
			break;
		case TOKEN_STRING:
			c->value.string = t->string;
			c->base.base.type = EXPRESSION_STRING;
			break;
		case TOKEN_NULL:
			c->value.null = NULL;
			c->base.base.type = EXPRESSION_NULL;
			break;
		default:
			/* you do nothing jon snow! */
			break;
	}

	return tokens;
}
accept_method(excall) {

}
pll_entry syntax_parse_excall(pll_entry tokens, pexcall* ret) {
	ptoken t = GETTKN(tokens);

	pexcall c = member_new(excall);
	member_sett(c, EXPRESSION_CALL, &excall_accept, t);
	*ret = c;
	c->identifier = t->string;

	tokens = NEXTTKN(tokens);
	tokens = syntax_parse_expressionlist(tokens, &c->arguments);

	return tokens;
}

membertype prefixunaryexpressions[][2] = {
	{ TOKEN_INCREMENT,	EXPRESSION_PREFIXINC },
	{ TOKEN_DECREMENT,	EXPRESSION_PREFIXDEC },
	{ TOKEN_NEGATION,	EXPRESSION_NEGATION },
	{ TOKEN_PLUS,		EXPRESSION_UNARYPLUS },
	{ TOKEN_MINUS,		EXPRESSION_UNARYMINUS },
	{ TOKEN_PLUS,		EXPRESSION_UNARYPLUS },
	{ TOKEN_NOT,		EXPRESSION_NOT },
	{ TOKEN_AND,		EXPRESSION_REFERENCE },

	{ TOKEN_EOL,		0 }
};
accept_method(exunary) {

}
pll_entry syntax_parse_exprefixunary(pll_entry tokens, pexunary* ret) {
	ptoken t = GETTKN(tokens);

	pexunary u = member_new(exunary);
	member_sett(u, 0, &exunary_accept, t);
	*ret = u;

	int i;
	for(i = 0;prefixunaryexpressions[i][0] != TOKEN_EOL;i++)
		if(t->base.type == prefixunaryexpressions[i][0]) {
			u->base.base.type = prefixunaryexpressions[i][1];
			break;
		}

	if(prefixunaryexpressions[i][0] == TOKEN_EOL)
		FATAL("Unexpected token '%s' on line %d, expected an unary operator", t->string, t->base.line)
	else {
		tokens = NEXTTKN(tokens);
		tokens = syntax_parse_exterminal(tokens, &u->expression);
	}

	return tokens;
}
accept_method(exbinary) {

}
pexbinary syntax_create_exbinary(ptoken operator, pexpression left, pexpression right) {
	pexbinary ret = member_new(exbinary);
	member_sett(ret, getexbntype(operator), &exbinary_accept, operator);

	ret->left = left;
	ret->right = right;

	return ret;
}
accept_method(externary) {

}
pll_entry syntax_parse_externary(pll_entry tokens, pexternary* ret) {
	pexternary ternary = member_new(externary);
	member_sett(ternary, EXPRESSION_TERNARY, &externary_accept, GETTKN(tokens));

	tokens = syntax_parse_expression(tokens, &ternary->iftrue);
	tokens = NEXTTKN(tokens);
	CHECK_TOKEN(GETTKN(tokens), TOKEN_COLON);

	tokens = NEXTTKN(tokens);
	tokens = syntax_parse_expression(tokens, &ternary->iffalse);

	return tokens;
}
accept_method(exbracket) {

}
pll_entry syntax_parse_exbracket(pll_entry tokens, pexbracket* expression) {
	pexbracket br = member_new(exbracket);
	member_sett(br, EXPRESSION_BRACKETS, &exbracket_accept, GETTKN(tokens));

	tokens = syntax_parse_expression(tokens, &br->expression);
	tokens = NEXTTKN(tokens);

	ptoken t = GETTKN(tokens);
	if(t->base.type != TOKEN_BREND)
		FATAL("Unexpected token '%s' on line %d, expected ')'", t->string, t->base.line);

	return tokens;
}
accept_method(exindex) {

}
pll_entry syntax_parse_exindex(pll_entry tokens, pexindex* ret) {
	ptoken t = GETTKN(tokens);
	tokens = NEXTTKN(tokens);

	pexindex i = member_new(exindex);
	member_sett(i, EXPRESSION_INDEX, &exindex_accept, t);

	CHECK_TOKEN(t, TOKEN_IBRSTART);
	tokens = syntax_parse_expression(tokens, &i->index);
	tokens = NEXTTKN(tokens);
	CHECK_TOKEN(t, TOKEN_IBREND);

	return tokens;
}
accept_method(exarray) {

}
pll_entry syntax_parse_exarray(pll_entry tokens, pexarray* ret) {
	pexarray a = member_new(exarray);
	member_sett(a, EXPRESSION_ARRAY, &exarray_accept, GETTKN(tokens));

	tokens = syntax_parse_expressionlist(tokens, &a->items);
	return tokens;
}
