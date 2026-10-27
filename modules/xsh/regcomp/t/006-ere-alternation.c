/*
 * ERE alternation, grouping and precedence.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.regcomp.006
 */
#include "rx.h"

static const struct rxc tab[] = {
	{ "a((bc)|d)", REG_EXTENDED, "abc", 0, 0, 3 },
	{ "a((bc)|d)", REG_EXTENDED, "ad", 0, 0, 2 },
	{ "a((bc)|d)", REG_EXTENDED, "ac", 0, -1, 0 },
	{ "(a|b|c)+", REG_EXTENDED, "xabcx", 0, 1, 4 },
	{ "ab|cd", REG_EXTENDED, "xcd", 0, 1, 3 },
	{ "ab|cd", REG_EXTENDED, "xbdacx", 0, -1, 0 },
	{ "abba|cde", REG_EXTENDED, "abbcde", 0, 3, 6 },
	{ "a(b|c)d", REG_EXTENDED, "acd", 0, 0, 3 },
	{ "a(b|c)d", REG_EXTENDED, "aed", 0, -1, 0 },
	{ "(a|b)*c", REG_EXTENDED, "ababc", 0, 0, 5 },
};

int main(void)
{
	pc_begin("XSH.regcomp.006");
	pc_test();
	rx_cases(tab, RX_N(tab));
	pc_pass();
}
