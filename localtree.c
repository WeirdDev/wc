#include <malloc.h>

#include "error.h"
#include "ll.h"
#include "expressiontree.h"
#include "localtree.h"

pll_entry syntax_parse_lsblock(pll_entry tokens, plinkedlist* members) {
	return tokens;
	//TODO: parse a {local space block} here
}
pll_entry syntax_parse_lsmember(pll_entry tokens, plsmember* ret) {
	return tokens;
	//TODO: parse a single local space member here
}
