/*
 * An ERE group repeated by a duplication symbol takes no needless null iteration.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.9.4-ere.004
 */
#include "rx.h"

#define EX REG_EXTENDED
static const struct rxs subs[] = {
	{ "(a*)*", EX, "aab", 0, 2, { 0, 2, 0, 2 } },
	{ "(a*)*", EX, "b", 0, 2, { 0, 0, 0, 0 } },
	{ "(a?)*", EX, "aab", 0, 2, { 0, 2, 1, 2 } },
	{ "(a*)*c", EX, "aac", 0, 2, { 0, 3, 0, 2 } },
	{ "(ab*)*", EX, "abbab", 0, 2, { 0, 5, 3, 5 } },
	{ "(a*b*)*", EX, "aabb", 0, 2, { 0, 4, 0, 4 } },
};
static const struct rxc tab[] = {
	{ "(a*){2}", EX, "a", 0, 0, 1 },
	{ "(a*){2}", EX, "", 0, 0, 0 },
};

int main(void)
{
	pc_begin("XBD.9.4-ere.004");
	pc_test();
	rx_subs(subs, RX_N(subs));
	rx_cases(tab, RX_N(tab));
	pc_pass();
}
