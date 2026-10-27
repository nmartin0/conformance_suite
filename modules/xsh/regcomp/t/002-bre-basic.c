/*
 * BRE literals, period, asterisk and concatenation.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.regcomp.002
 */
#include "rx.h"

static const struct rxc tab[] = {
	{ "bb*", 0, "abbbc", 0, 1, 4 },
	{ "abc", 0, "xxabcxx", 0, 2, 5 },
	{ "ab", 0, "aab", 0, 1, 3 },
	{ "a.c", 0, "xabcx", 0, 1, 4 },
	{ "a.c", 0, "ac", 0, -1, 0 },
	{ "ab*c", 0, "ac", 0, 0, 2 },
	{ "ab*c", 0, "abbbc", 0, 0, 5 },
	{ "a*", 0, "baaa", 0, 0, 0 },
	{ "a*", 0, "aab", 0, 0, 2 },
	{ "[ab]*", 0, "ab", 0, 0, 2 },
	{ "[ab][ab]", 0, "ab", 0, 0, 2 },
	{ "x", 0, "", 0, -1, 0 },
	{ "abc", 0, "abd", 0, -1, 0 },
	{ "bb*", 0, "abbbc", 0, 1, 4 },
	{ "bb*", REG_EXTENDED, "abbbc", 0, 1, 4 },
};

int main(void)
{
	pc_begin("XSH.regcomp.002");
	pc_test();
	rx_cases(tab, RX_N(tab));
	pc_pass();
}
