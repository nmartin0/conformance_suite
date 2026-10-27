/*
 * BRE anchoring.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.regcomp.007
 */
#include "rx.h"

static const struct rxc tab[] = {
	{ "^ab", 0, "abcdef", 0, 0, 2 },
	{ "^ab", 0, "cdefab", 0, -1, 0 },
	{ "ab$", 0, "cdefab", 0, 4, 6 },
	{ "ab$", 0, "abcdef", 0, -1, 0 },
	{ "^abcdef$", 0, "abcdef", 0, 0, 6 },
	{ "^abcdef$", 0, "abcdefg", 0, -1, 0 },
	{ "^abcdef$", 0, "xabcdef", 0, -1, 0 },
	{ "^$", 0, "", 0, 0, 0 },
	{ "^$", 0, "a", 0, -1, 0 },
	{ "a^b", 0, "xa^bx", 0, 1, 4 },
	{ "a$b", 0, "xa$bx", 0, 1, 4 },
	{ "$a", 0, "x$ay", 0, 1, 3 },
};

int main(void)
{
	pc_begin("XSH.regcomp.007");
	pc_test();
	rx_cases(tab, RX_N(tab));
	pc_pass();
}
