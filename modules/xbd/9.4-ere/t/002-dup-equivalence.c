/*
 * Repetition forms equal their expansions.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.9.4-ere.002
 */
#include "rx.h"

static const char *const str[] = {
	"", "a", "ab", "abab", "xababy", "aaaa", "bbb", "ba", "aab", "abcdefabcdef", "aaaaa", NULL
};
static const char *const pairs[][2] = {
	{ "[ab]+", "[ab][ab]*" },
	{ "(ab)+", "(ab)(ab)*" },
	{ "a{2}", "aa" },
	{ "a{2,}", "aaa*" },
	{ "a{1,3}", "a(a(a)?)?" },
	{ "a?b", "(ab|b)" },
	{ "a*b", "(a+b|b)" },
	{ "b{0}a", "a" },
};
static const struct rxs subs[] = {
	{ "cd", REG_EXTENDED, "abcdefabcdef", 0, 1, { 2, 4 } },
	{ "(cd)", REG_EXTENDED, "abcdefabcdef", 0, 2, { 2, 4, 2, 4 } },
};

int main(void)
{
	size_t i;

	pc_begin("XBD.9.4-ere.002");
	pc_test();
	for (i = 0; i < RX_N(pairs); i++)
		rx_same(pairs[i][0], pairs[i][1], REG_EXTENDED, str);
	rx_subs(subs, RX_N(subs));
	pc_pass();
}
