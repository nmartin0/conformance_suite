/*
 * Subpatterns match the longest possible string from left to right, an empty match counting as longer than none.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.regcomp.026
 */
#include "rx.h"

static const struct rxs tab[] = {
	{ "\\(.*\\).*", 0, "abcdef", 0, 2, { 0, 6, 0, 6 } },
	{ "\\(a*\\)*", 0, "bc", 0, 2, { 0, 0, 0, 0 } },
	{ "(a|ab)(c|bcd)(d*)", REG_EXTENDED, "abcd", 0, 4, { 0, 4, 0, 2, 2, 3, 3, 4 } },
	{ "(a*)(a*)", REG_EXTENDED, "aaa", 0, 3, { 0, 3, 0, 3, 3, 3 } },
	{ "(.*)(.*)", REG_EXTENDED, "xy", 0, 3, { 0, 2, 0, 2, 2, 2 } },
};

int main(void)
{
	pc_begin("XSH.regcomp.026");
	pc_test();
	rx_subs(tab, RX_N(tab));
	pc_pass();
}
