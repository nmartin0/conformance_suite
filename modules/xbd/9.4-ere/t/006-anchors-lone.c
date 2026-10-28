/*
 * Lone anchors and anchors inside groups next to alternation.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.9.4-ere.006
 */
#include "rx.h"

#define EX REG_EXTENDED
static const struct rxc tab[] = {
	{ "^", EX, "abc", 0, 0, 0 },
	{ "$", EX, "abc", 0, 3, 3 },
	{ "^", EX, "", 0, 0, 0 },
	{ "$", EX, "", 0, 0, 0 },
	{ "^$|x", EX, "ab", 0, -1, 0 },
	{ "^$|x", EX, "", 0, 0, 0 },
	{ "^$|x", EX, "ax", 0, 1, 2 },
	{ "(^|x)a", EX, "ba", 0, -1, 0 },
	{ "a($|b)", EX, "ac", 0, -1, 0 },
};
static const struct rxs subs[] = {
	{ "(^|x)a", EX, "a", 0, 2, { 0, 1, 0, 0 } },
	{ "(^|x)a", EX, "xa", 0, 2, { 0, 2, 0, 1 } },
	{ "a($|b)", EX, "a", 0, 2, { 0, 1, 1, 1 } },
	{ "a($|b)", EX, "ab", 0, 2, { 0, 2, 1, 2 } },
};

int main(void)
{
	pc_begin("XBD.9.4-ere.006");
	pc_test();
	rx_cases(tab, RX_N(tab));
	rx_subs(subs, RX_N(subs));
	pc_pass();
}
