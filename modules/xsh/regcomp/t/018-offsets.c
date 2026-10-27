/*
 * pmatch[0] is the whole match; pmatch[i] belongs to the ith opening parenthesis; offsets count from the start of the string.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.regcomp.018
 */
#include "rx.h"

static const struct rxs tab[] = {
	{ "(a)(b)(c)", REG_EXTENDED, "xabcx", 0, 4, { 1, 4, 1, 2, 2, 3, 3, 4 } },
	{ "((a)(b))", REG_EXTENDED, "ab", 0, 4, { 0, 2, 0, 2, 0, 1, 1, 2 } },
	{ "(a(b)(c))(d)", REG_EXTENDED, "abcd", 0, 5, { 0, 4, 0, 3, 1, 2, 2, 3, 3, 4 } },
	{ "\\(x\\)\\(y\\)", 0, "axyb", 0, 3, { 1, 3, 1, 2, 2, 3 } },
	{ "\\(a\\(b\\)\\)", 0, "zab", 0, 3, { 1, 3, 1, 3, 2, 3 } },
	{ "(a)(b)", REG_EXTENDED, "zzab", 0, 3, { 2, 4, 2, 3, 3, 4 } },
};

int main(void)
{
	pc_begin("XSH.regcomp.018");
	pc_test();
	rx_subs(tab, RX_N(tab));
	pc_pass();
}
