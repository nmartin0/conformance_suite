/*
 * A repeated subexpression takes no needless null iteration.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.9.3-bre.003
 */
#include "rx.h"

static const struct rxs subs[] = {
	{ "\\(a*\\)*", 0, "aab", 0, 2, { 0, 2, 0, 2 } },
	{ "\\(a*\\)*", 0, "b", 0, 2, { 0, 0, 0, 0 } },
	{ "\\(a*\\)*c", 0, "aac", 0, 2, { 0, 3, 0, 2 } },
	{ "\\(ab*\\)*", 0, "abbab", 0, 2, { 0, 5, 3, 5 } },
	{ "\\(a*b*\\)*", 0, "aabb", 0, 2, { 0, 4, 0, 4 } },
};
static const struct rxc tab[] = {
	{ "\\(x*\\)\\{2\\}", 0, "x", 0, 0, 1 },
	{ "\\(x*\\)\\{2\\}", 0, "", 0, 0, 0 },
};

int main(void)
{
	pc_begin("XBD.9.3-bre.003");
	pc_test();
	rx_subs(subs, RX_N(subs));
	rx_cases(tab, RX_N(tab));
	pc_pass();
}
