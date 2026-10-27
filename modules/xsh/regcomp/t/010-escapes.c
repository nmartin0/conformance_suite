/*
 * Escaped special characters match themselves (BRE and ERE quoted characters).
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.regcomp.010
 */
#include "rx.h"

static const struct rxc tab[] = {
	/* BRE */
	{ "a\\.c", 0, "abc a.c", 0, 4, 7 },
	{ "a\\.c", 0, "abc", 0, -1, 0 },
	{ "a\\*", 0, "aa*", 0, 1, 3 },
	{ "\\[x\\]", 0, "a[x]b", 0, 1, 4 },
	{ "\\\\", 0, "a\\b", 0, 1, 2 },
	{ "\\^a", 0, "b^a", 0, 1, 3 },
	{ "a\\$", 0, "a$", 0, 0, 2 },
	{ "a\\$", 0, "a", 0, -1, 0 },
	/* ERE */
	{ "x\\(a\\)y", REG_EXTENDED, "x(a)y", 0, 0, 5 },
	{ "a\\|b", REG_EXTENDED, "ab a|b", 0, 3, 6 },
	{ "a\\*", REG_EXTENDED, "aa*", 0, 1, 3 },
	{ "a\\+", REG_EXTENDED, "aa+", 0, 1, 3 },
	{ "a\\?", REG_EXTENDED, "aa?", 0, 1, 3 },
	{ "a\\{2\\}", REG_EXTENDED, "aa a{2}", 0, 3, 7 },
	{ "\\^a", REG_EXTENDED, "b^a", 0, 1, 3 },
	{ "a\\$", REG_EXTENDED, "a$", 0, 0, 2 },
	{ "a\\.c", REG_EXTENDED, "abc a.c", 0, 4, 7 },
	{ "\\[x\\]", REG_EXTENDED, "a[x]b", 0, 1, 4 },
	{ "\\\\", REG_EXTENDED, "a\\b", 0, 1, 2 },
};

int main(void)
{
	pc_begin("XSH.regcomp.010");
	pc_test();
	rx_cases(tab, RX_N(tab));
	pc_pass();
}
