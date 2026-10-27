/*
 * Elements of pmatch up to nmatch-1 beyond the subexpressions are -1 in both members.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.regcomp.019
 */
#include "rx.h"

static const struct rxs tab[] = {
	{ "(a)", REG_EXTENDED, "a", 0, 4, { 0, 1, 0, 1, -1, -1, -1, -1 } },
	{ "abc", 0, "xabcx", 0, 3, { 1, 4, -1, -1, -1, -1 } },
	{ "(a)(b)", REG_EXTENDED, "ab", 0, 6, { 0, 2, 0, 1, 1, 2, -1, -1, -1, -1, -1, -1 } },
	{ "\\(a\\)", 0, "a", 0, 8, { 0, 1, 0, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 } },
};

int main(void)
{
	pc_begin("XSH.regcomp.019");
	pc_test();
	rx_subs(tab, RX_N(tab));
	pc_pass();
}
