/*
 * In a BRE an asterisk first in the expression, after a leading circumflex, or first in a subexpression is ordinary.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.regcomp.009
 */
#include "rx.h"

static const struct rxc tab[] = {
	{ "*a", 0, "x*a", 0, 1, 3 },
	{ "*a", 0, "xaa", 0, -1, 0 },
	{ "*", 0, "a*b", 0, 1, 2 },
	{ "^*a", 0, "*a", 0, 0, 2 },
	{ "^*a", 0, "a", 0, -1, 0 },
	{ "\\(*a\\)", 0, "x*ay", 0, 1, 3 },
	{ "\\(*a\\)", 0, "xaay", 0, -1, 0 },
};

int main(void)
{
	pc_begin("XSH.regcomp.009");
	pc_test();
	rx_cases(tab, RX_N(tab));
	pc_pass();
}
