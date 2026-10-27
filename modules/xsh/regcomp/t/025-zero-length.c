/*
 * A subexpression matching the empty string reports the offset of what follows it, in both members.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.regcomp.025
 */
#include "rx.h"

static const struct rxs tab[] = {
	{ "(x*)b", REG_EXTENDED, "ab", 0, 2, { 1, 2, 1, 1 } },
	{ "b(x*)", REG_EXTENDED, "ab", 0, 2, { 1, 2, 2, 2 } },
	{ "a(x*)b", REG_EXTENDED, "ab", 0, 2, { 0, 2, 1, 1 } },
	{ "\\(x*\\)", 0, "ab", 0, 2, { 0, 0, 0, 0 } },
	{ "(a*)(b*)c", REG_EXTENDED, "c", 0, 3, { 0, 1, 0, 0, 0, 0 } },
};

int main(void)
{
	pc_begin("XSH.regcomp.025");
	pc_test();
	rx_subs(tab, RX_N(tab));
	pc_pass();
}
