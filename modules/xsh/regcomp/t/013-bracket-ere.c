/*
 * ERE bracket expressions: the operators are ordinary inside, a circumflex inside is not an anchor.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.regcomp.013
 */
#include "rx.h"

static const struct rxc tab[] = {
	{ "^[a^]", REG_EXTENDED, "^", 0, 0, 1 },
	{ "[a^]$", REG_EXTENDED, "x^", 0, 1, 2 },
	{ "x[.]y", REG_EXTENDED, "x.y", 0, 0, 3 },
	{ "x[.]y", REG_EXTENDED, "xzy", 0, -1, 0 },
	{ "[]a]+", REG_EXTENDED, "x]a]y", 0, 1, 4 },
};

int main(void)
{
	pc_begin("XSH.regcomp.013");
	pc_test();
	rx_cases(tab, RX_N(tab));
	rx_set("[(*+?{|$.^]", REG_EXTENDED, "(*+?{|$.^", "az)}");
	rx_set("[^(*+?{|$.^]", REG_EXTENDED, "az)}", "(*+?{|$.^");
	rx_set("[\\]", REG_EXTENDED, "\\", "n.");
	rx_set("[[]", REG_EXTENDED, "[", "a");
	pc_pass();
}
