/*
 * A back-reference can be repeated by an asterisk or an interval.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.9.3-bre.002
 */
#include "rx.h"

static const struct rxc tab[] = {
	{ "\\(a\\)\\1*", 0, "aaaa", 0, 0, 4 },
	{ "\\(a\\)\\1*", 0, "ab", 0, 0, 1 },
	{ "\\(ab\\)\\1\\{2\\}", 0, "ababab", 0, 0, 6 },
	{ "\\(ab\\)\\1\\{2\\}", 0, "abab", 0, -1, 0 },
	{ "\\(ab\\)\\1\\{2\\}", 0, "abababab", 0, 0, 6 },
	{ "\\(.\\)\\1\\{2,\\}", 0, "xaaaay", 0, 1, 5 },
	{ "\\(.\\)\\1\\{2,\\}", 0, "xaay", 0, -1, 0 },
	{ "\\(.\\)\\1\\{0,1\\}b", 0, "xaab", 0, 1, 4 },
};
static const struct rxs subs[] = {
	{ "\\(a\\)\\1*", 0, "aaaa", 0, 2, { 0, 4, 0, 1 } },
};

int main(void)
{
	pc_begin("XBD.9.3-bre.002");
	pc_test();
	rx_cases(tab, RX_N(tab));
	rx_subs(subs, RX_N(subs));
	pc_pass();
}
