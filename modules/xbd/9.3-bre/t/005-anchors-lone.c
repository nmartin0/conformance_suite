/*
 * Lone anchors and anchors around subexpressions.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.9.3-bre.005
 */
#include "rx.h"

static const struct rxc tab[] = {
	{ "^", 0, "abc", 0, 0, 0 },
	{ "$", 0, "abc", 0, 3, 3 },
	{ "^", 0, "", 0, 0, 0 },
	{ "$", 0, "", 0, 0, 0 },
	{ "x*$", 0, "axx", 0, 1, 3 },
	{ "^x*", 0, "xxa", 0, 0, 2 },
	{ "b$", 0, "abb", 0, 2, 3 },
	{ "^a", 0, "aa", 0, 0, 1 },
	{ "^.", 0, "ab", 0, 0, 1 },
	{ ".$", 0, "ab", 0, 1, 2 },
	{ "^\\(a\\)", 0, "ab", 0, 0, 1 },
	{ "^\\(a\\)", 0, "ba", 0, -1, 0 },
	{ "\\(a\\)$", 0, "ba", 0, 1, 2 },
	{ "\\(a\\)$", 0, "ab", 0, -1, 0 },
};
static const struct rxs subs[] = {
	{ "\\(a\\)$", 0, "ba", 0, 2, { 1, 2, 1, 2 } },
};

int main(void)
{
	pc_begin("XBD.9.3-bre.005");
	pc_test();
	rx_cases(tab, RX_N(tab));
	rx_subs(subs, RX_N(subs));
	pc_pass();
}
