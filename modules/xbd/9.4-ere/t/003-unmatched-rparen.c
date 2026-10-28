/*
 * A right parenthesis with no left one before it is an ordinary character.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.9.4-ere.003
 */
#include "rx.h"

#define EX REG_EXTENDED
static const struct rxc tab[] = {
	{ "a)", EX, "a)", 0, 0, 2 },
	{ "a)", EX, "a", 0, -1, 0 },
	{ ")", EX, "x)", 0, 1, 2 },
};
static const struct rxs subs[] = {
	{ "(a))", EX, "a)", 0, 2, { 0, 2, 0, 1 } },
};

int main(void)
{
	pc_begin("XBD.9.4-ere.003");
	pc_test();
	rx_cases(tab, RX_N(tab));
	rx_subs(subs, RX_N(subs));
	pc_pass();
}
