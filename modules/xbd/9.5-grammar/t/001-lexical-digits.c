/*
 * Back-references are one digit; digits outside an interval are ordinary.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.9.5-grammar.001
 */
#include "rx.h"

#define EX REG_EXTENDED
static const struct rxc tab[] = {
	{ "\\(a\\)\\10", 0, "aa0", 0, 0, 3 },
	{ "\\(a\\)\\10", 0, "aa", 0, -1, 0 },
	{ "1\\{2\\}", 0, "x11y", 0, 1, 3 },
	{ "12\\{2\\}", 0, "122", 0, 0, 3 },
	{ "12\\{2\\}", 0, "1212", 0, -1, 0 },
	{ "0\\{0\\}1", 0, "1", 0, 0, 1 },
	{ "a\\{1,2\\}0", 0, "aa0", 0, 0, 3 },
	{ "1{2}", EX, "x11y", 0, 1, 3 },
	{ "12{2}", EX, "122", 0, 0, 3 },
	{ "12{2}", EX, "1212", 0, -1, 0 },
	{ "0{0}1", EX, "1", 0, 0, 1 },
	{ "a{1,2}0", EX, "aa0", 0, 0, 3 },
	{ "a{1,2}12", EX, "aa12", 0, 0, 4 },
};
static const struct rxs subs[] = {
	{ "\\(a\\)\\10", 0, "aa0", 0, 2, { 0, 3, 0, 1 } },
};

int main(void)
{
	pc_begin("XBD.9.5-grammar.001");
	pc_test();
	rx_cases(tab, RX_N(tab));
	rx_subs(subs, RX_N(subs));
	pc_pass();
}
