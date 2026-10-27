/*
 * A subexpression that took part in the match several times reports the last time.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.regcomp.022
 */
#include "rx.h"

static const struct rxs tab[] = {
	{ "(a|b)*", REG_EXTENDED, "abab", 0, 2, { 0, 4, 3, 4 } },
	{ "(ab)+", REG_EXTENDED, "ababab", 0, 2, { 0, 6, 4, 6 } },
	{ "\\(ab*\\)*", 0, "abbab", 0, 2, { 0, 5, 3, 5 } },
	{ "\\(a\\)\\{3\\}", 0, "aaa", 0, 2, { 0, 3, 2, 3 } },
	{ "(.)*", REG_EXTENDED, "xyz", 0, 2, { 0, 3, 2, 3 } },
	{ "(a){2,}", REG_EXTENDED, "xaaaa", 0, 2, { 1, 5, 4, 5 } },
};

int main(void)
{
	pc_begin("XSH.regcomp.022");
	pc_test();
	rx_subs(tab, RX_N(tab));
	pc_pass();
}
