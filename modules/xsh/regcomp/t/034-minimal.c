/*
 * REG_MINIMAL makes leftmost shortest the default and ? selects the longest.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.regcomp.034
 */
#include "rx.h"

#define MX (REG_EXTENDED | REG_MINIMAL)
static const struct rxc tab[] = {
	{ "a+", MX, "aaa", 0, 0, 1 },
	{ "a*", MX, "aaa", 0, 0, 0 },
	{ "a?", MX, "a", 0, 0, 0 },
	{ "a{2,3}", MX, "aaaa", 0, 0, 2 },
	{ ".*c", MX, "abc abc", 0, 0, 3 },
	{ "b+", MX, "abbb", 0, 1, 2 },
	{ "a+?", MX, "aaa", 0, 0, 3 },
	{ ".*?c", MX, "abc abc", 0, 0, 7 },
	{ "a??", MX, "a", 0, 0, 1 },
	{ "a{2,3}?", MX, "aaaa", 0, 0, 3 },
	{ "a*?", MX, "aaa", 0, 0, 3 },
};

int main(void)
{
	pc_begin("XSH.regcomp.034");
	pc_test();
	pc_check(REG_MINIMAL != 0 && (REG_MINIMAL & (REG_EXTENDED | REG_ICASE | REG_NOSUB | REG_NEWLINE)) == 0,
	    "REG_MINIMAL is zero or overlaps another compile flag");
	rx_cases(tab, RX_N(tab));
	pc_pass();
}
