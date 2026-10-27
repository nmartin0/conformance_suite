/*
 * BRE back-references, including the examples the standard gives.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.regcomp.017
 */
#include "rx.h"

static const struct rxc tab[] = {
	{ "^\\(.*\\)\\1$", 0, "abab", 0, 0, 4 },
	{ "^\\(.*\\)\\1$", 0, "aa", 0, 0, 2 },
	{ "^\\(.*\\)\\1$", 0, "aba", 0, -1, 0 },
	{ "^\\(.*\\)\\1$", 0, "", 0, 0, 0 },
	{ "\\(a\\)*\\1", 0, "a", 0, -1, 0 },
	{ "\\(a\\(b\\)*\\)*\\2", 0, "abab", 0, -1, 0 },
	{ "^\\(ab*\\)*\\1$", 0, "ababbabb", 0, 0, 8 },
	{ "^\\(ab*\\)*\\1$", 0, "ababbab", 0, -1, 0 },
	{ "\\(a*\\)b\\1", 0, "aabaa", 0, 0, 5 },
	{ "\\(a*\\)b\\1", 0, "aaba", 0, 1, 4 },
	{ "\\(.\\)\\1", 0, "abccd", 0, 2, 4 },
	{ "\\(.\\)\\1", 0, "abcd", 0, -1, 0 },
};
static const struct rxs subs[] = {
	{ "\\(a\\)\\(b\\)\\2\\1", 0, "abba", 0, 3, { 0, 4, 0, 1, 1, 2 } },
};

int main(void)
{
	pc_begin("XSH.regcomp.017");
	pc_test();
	rx_cases(tab, RX_N(tab));
	rx_subs(subs, RX_N(subs));
	pc_pass();
}
