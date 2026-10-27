/*
 * BRE interval expressions.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.regcomp.004
 */
#include "rx.h"

#define S "abababccccccd"
static const struct rxc tab[] = {
	{ "c\\{3\\}", 0, S, 0, 6, 9 },
	{ "\\(ab\\)\\{4,\\}", 0, S, 0, -1, 0 },
	{ "c\\{1,3\\}d", 0, S, 0, 9, 13 },
	{ "a\\{0\\}b", 0, "aab", 0, 2, 3 },
	{ "a\\{2,\\}", 0, "baaaa", 0, 1, 5 },
	{ "a\\{2,3\\}", 0, "aaaa", 0, 0, 3 },
	{ "a\\{2\\}", 0, "a", 0, -1, 0 },
	{ "\\(ab\\)\\{2\\}", 0, "ababab", 0, 0, 4 },
	{ "[ab]\\{3\\}", 0, "xabbay", 0, 1, 4 },
	{ ".\\{2\\}", 0, "abc", 0, 0, 2 },
	{ "\\(ab\\)\\{2,\\}", 0, "xababab", 0, 1, 7 },
};

int main(void)
{
	pc_begin("XSH.regcomp.004");
	pc_test();
	rx_cases(tab, RX_N(tab));
	pc_pass();
}
