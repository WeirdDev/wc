#include "member.h"
#include "lexer.h"

void member_set(void* member, membertype type, accept_method_ptr accept, int line) {
	pmember m = (pmember)member;
	m->type = type;
	m->accept = accept;
	m->line = line;
}
void member_sett(void* member, membertype type, accept_method_ptr accept, ptoken t) {
	member_set(member, type, accept, t->base.line);
}
