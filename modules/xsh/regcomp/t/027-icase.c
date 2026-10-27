/*
 * REG_ICASE: every case variation of a matching string also matches.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.regcomp.027
 */
#include "rx.h"

static const struct rxc tab[] = {
	{ "abc", REG_ICASE, "xAbCx", 0, 1, 4 },
	{ "ABC", REG_ICASE | REG_EXTENDED, "xaBcx", 0, 1, 4 },
	{ "abc", 0, "ABC", 0, -1, 0 },
	{ "[a-c]x", REG_ICASE, "Bx", 0, 0, 2 },
	{ "[[:lower:]]", REG_ICASE, "A", 0, 0, 1 },
	{ "[[:upper:]]", REG_ICASE, "a", 0, 0, 1 },
	{ "\\(a\\)\\1", REG_ICASE, "aA", 0, 0, 2 },
	{ "\\(a\\)\\1", REG_ICASE, "Aa", 0, 0, 2 },
	{ "\\(a\\)\\1", REG_ICASE, "AA", 0, 0, 2 },
	{ "\\(a\\)\\1", 0, "aA", 0, -1, 0 },
	{ "a", REG_ICASE, "xyz", 0, -1, 0 },
	{ "(ab)+", REG_ICASE | REG_EXTENDED, "aBAb", 0, 0, 4 },
};

int main(void)
{
	pc_begin("XSH.regcomp.027");
	pc_test();
	rx_cases(tab, RX_N(tab));
	pc_pass();
}
