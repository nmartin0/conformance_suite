/*
 * Whole match longest first, then each subpattern longest left to right.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.9.1-definitions.003
 */
#include "rx.h"

#define EX REG_EXTENDED
static const struct rxs subs[] = {
	{ "(a*)(b|abc)", EX, "abc", 0, 3, { 0, 3, 0, 0, 0, 3 } },
	{ "(ab|a)(bc|c)?", EX, "abc", 0, 3, { 0, 3, 0, 2, 2, 3 } },
	{ "(a*)(ab)*(b*)", EX, "aabb", 0, 4, { 0, 4, 0, 2, -1, -1, 2, 4 } },
	{ "\\(.*\\)\\(.*\\)", 0, "abc", 0, 3, { 0, 3, 0, 3, 3, 3 } },
	{ "(.?)(.?)(.?)", EX, "ab", 0, 4, { 0, 2, 0, 1, 1, 2, 2, 2 } },
};

int main(void)
{
	pc_begin("XBD.9.1-definitions.003");
	pc_test();
	rx_subs(subs, RX_N(subs));
	pc_pass();
}
