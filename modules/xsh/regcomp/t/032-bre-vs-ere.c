/*
 * Basic expressions take + ? | ( ) { } literally; extended ones give them their operator meaning.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.regcomp.032
 */
#include "rx.h"

static const struct rxc tab[] = {
	{ "a+", 0, "xa+y", 0, 1, 3 },
	{ "a+", REG_EXTENDED, "xaay", 0, 1, 3 },
	{ "(ab)", 0, "x(ab)y", 0, 1, 5 },
	{ "(ab)", REG_EXTENDED, "x(ab)y", 0, 2, 4 },
	{ "a|b", 0, "xa|b", 0, 1, 4 },
	{ "a|b", 0, "b", 0, -1, 0 },
	{ "a|b", REG_EXTENDED, "b", 0, 0, 1 },
	{ "a{2}", 0, "a{2}", 0, 0, 4 },
	{ "a{2}", 0, "aaa", 0, -1, 0 },
	{ "a{2}", REG_EXTENDED, "aaa", 0, 0, 2 },
	{ "a?", 0, "xa?", 0, 1, 3 },
	{ "a?", REG_EXTENDED, "b", 0, 0, 0 },
	{ "\\(ab\\)*", 0, "ababx", 0, 0, 4 },
};

int main(void)
{
	pc_begin("XSH.regcomp.032");
	pc_test();
	rx_cases(tab, RX_N(tab));
	pc_pass();
}
