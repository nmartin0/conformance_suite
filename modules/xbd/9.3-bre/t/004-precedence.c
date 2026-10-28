/*
 * BRE precedence: duplication, escapes, brackets and anchors.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.9.3-bre.004
 */
#include "rx.h"

static const struct rxc tab[] = {
	{ "ab\\{2\\}", 0, "ababb", 0, 2, 5 },
	{ "ab\\{2\\}", 0, "abab", 0, -1, 0 },
	{ "\\(ab\\)\\{2\\}", 0, "ababab", 0, 0, 4 },
	{ "a\\.*b", 0, "xa...b", 0, 1, 6 },
	{ "a\\.*b", 0, "ab", 0, 0, 2 },
	{ "a\\.*b", 0, "axb", 0, -1, 0 },
	{ "a\\**b", 0, "a**b", 0, 0, 4 },
	{ "\\[a]", 0, "x[a]", 0, 1, 4 },
	{ "\\[a]", 0, "xa]", 0, -1, 0 },
	{ "[ab]*c", 0, "ababc", 0, 0, 5 },
	{ "^ab*", 0, "abbb", 0, 0, 4 },
	{ "ab*$", 0, "xabb", 0, 1, 4 },
	{ "ab\\{0\\}c", 0, "ac", 0, 0, 2 },
	{ "ab\\{0\\}c", 0, "abc", 0, -1, 0 },
};
static const struct rxs subs[] = {
	{ "a\\(b\\)*c", 0, "abbc", 0, 2, { 0, 4, 2, 3 } },
};

int main(void)
{
	pc_begin("XBD.9.3-bre.004");
	pc_test();
	rx_cases(tab, RX_N(tab));
	rx_subs(subs, RX_N(subs));
	pc_pass();
}
