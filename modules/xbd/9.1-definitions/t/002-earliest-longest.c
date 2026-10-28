/*
 * The earliest match wins, then the longest from that position.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.9.1-definitions.002
 */
#include "rx.h"

#define EX REG_EXTENDED
static const struct rxc tab[] = {
	{ "a|bcd", EX, "abcd", 0, 0, 1 },
	{ "bcd|a", EX, "abcd", 0, 0, 1 },
	{ "d|bcd", EX, "abcd", 0, 1, 4 },
	{ "bcd|d", EX, "abcd", 0, 1, 4 },
	{ "xyz|y", EX, "xyz", 0, 0, 3 },
	{ "y|xyz", EX, "xyz", 0, 0, 3 },
	{ "aa*b|a", EX, "aaac", 0, 0, 1 },
	{ "(foo|foobar)(bar)?", EX, "xfoobarz", 0, 1, 7 },
	{ "x*", 0, "abxxx", 0, 0, 0 },
	{ "x*", EX, "abxxx", 0, 0, 0 },
	{ "b*", 0, "abb", 0, 0, 0 },
	{ "xx*", 0, "abxxx", 0, 2, 5 },
	{ "b*c", 0, "abbc", 0, 1, 4 },
};

int main(void)
{
	pc_begin("XBD.9.1-definitions.002");
	pc_test();
	rx_cases(tab, RX_N(tab));
	pc_pass();
}
