/*
 * ERE anchoring.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.regcomp.008
 */
#include "rx.h"

static const struct rxc tab[] = {
	{ "^ab", REG_EXTENDED, "abcdef", 0, 0, 2 },
	{ "^ab", REG_EXTENDED, "cdefab", 0, -1, 0 },
	{ "(^ab)", REG_EXTENDED, "abcdef", 0, 0, 2 },
	{ "(^ab)", REG_EXTENDED, "cdefab", 0, -1, 0 },
	{ "ef$", REG_EXTENDED, "abcdef", 0, 4, 6 },
	{ "ef$", REG_EXTENDED, "cdefab", 0, -1, 0 },
	{ "(ef$)", REG_EXTENDED, "abcdef", 0, 4, 6 },
	{ "(ef$)", REG_EXTENDED, "cdefab", 0, -1, 0 },
	{ "a^b", REG_EXTENDED, "a^b", 0, -1, 0 },
	{ "a^b", REG_EXTENDED, "ab", 0, -1, 0 },
	{ "e$f", REG_EXTENDED, "ef", 0, -1, 0 },
	{ "e$f", REG_EXTENDED, "e$f", 0, -1, 0 },
	{ "^abcdef$", REG_EXTENDED, "abcdef", 0, 0, 6 },
	{ "^abcdef$", REG_EXTENDED, "abcdefg", 0, -1, 0 },
	{ "a|^b", REG_EXTENDED, "xb", 0, -1, 0 },
	{ "a|^b", REG_EXTENDED, "bx", 0, 0, 1 },
	{ "^$", REG_EXTENDED, "", 0, 0, 0 },
};

int main(void)
{
	pc_begin("XSH.regcomp.008");
	pc_test();
	rx_cases(tab, RX_N(tab));
	pc_pass();
}
