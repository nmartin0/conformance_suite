/*
 * A subexpression that did not take part in a successful match has both offsets -1.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.regcomp.023
 */
#include "rx.h"

static const struct rxs tab[] = {
	{ "(a)?b", REG_EXTENDED, "b", 0, 2, { 0, 1, -1, -1 } },
	{ "(a)?b", REG_EXTENDED, "ab", 0, 2, { 0, 2, 0, 1 } },
	{ "(a)*b", REG_EXTENDED, "b", 0, 2, { 0, 1, -1, -1 } },
	{ "(a){0,1}b", REG_EXTENDED, "b", 0, 2, { 0, 1, -1, -1 } },
	{ "\\(a\\)*b", 0, "b", 0, 2, { 0, 1, -1, -1 } },
	{ "\\(a\\)\\{0,1\\}b", 0, "b", 0, 2, { 0, 1, -1, -1 } },
	{ "(a)|(b)", REG_EXTENDED, "b", 0, 3, { 0, 1, -1, -1, 0, 1 } },
	{ "(a)|(b)", REG_EXTENDED, "a", 0, 3, { 0, 1, 0, 1, -1, -1 } },
	{ "x((a)|(b))y", REG_EXTENDED, "xby", 0, 4, { 0, 3, 1, 2, -1, -1, 1, 2 } },
};

int main(void)
{
	pc_begin("XSH.regcomp.023");
	pc_test();
	rx_subs(tab, RX_N(tab));
	pc_pass();
}
