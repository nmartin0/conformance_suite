/*
 * A backslash that is itself escaped starts no further escape sequence.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.9.1-definitions.004
 */
#include "rx.h"

#define EX REG_EXTENDED
static const struct rxc tab[] = {
	/* BRE: a, backslash, digit 1: the digit is not a back-reference */
	{ "\\(a\\)\\\\1", 0, "a\\1", 0, 0, 3 },
	{ "\\(a\\)\\\\1", 0, "aa", 0, -1, 0 },
	{ "a\\\\*b", 0, "ab", 0, 0, 2 },
	{ "a\\\\*b", 0, "a\\\\b", 0, 0, 4 },
	{ "\\\\.", 0, "\\x", 0, 0, 2 },
	{ "\\\\.", 0, "ab", 0, -1, 0 },
	{ "\\\\.", 0, "\\", 0, -1, 0 },
	/* ERE */
	{ "\\\\.", EX, "\\b", 0, 0, 2 },
	{ "\\\\.", EX, "..", 0, -1, 0 },
	{ "a\\\\+", EX, "a\\\\", 0, 0, 3 },
	{ "\\\\|b", EX, "xb", 0, 1, 2 },
	{ "\\\\|b", EX, "x\\", 0, 1, 2 },
};
static const struct rxs subs[] = {
	{ "\\\\\\(a\\)", 0, "\\a", 0, 2, { 0, 2, 1, 2 } },
	{ "\\\\(a)", EX, "\\a", 0, 2, { 0, 2, 1, 2 } },
};

int main(void)
{
	pc_begin("XBD.9.1-definitions.004");
	pc_test();
	rx_cases(tab, RX_N(tab));
	rx_subs(subs, RX_N(subs));
	pc_pass();
}
