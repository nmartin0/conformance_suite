/*
 * In an ERE a duplication symbol followed by ? selects the leftmost shortest match.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.regcomp.033
 */
#include "rx.h"

#define EX REG_EXTENDED
static const struct rxc tab[] = {
	{ ".*c", EX, "abc abc", 0, 0, 7 },
	{ ".*?c", EX, "abc abc", 0, 0, 3 },
	{ "a+?", EX, "aaa", 0, 0, 1 },
	{ "a*?", EX, "aaa", 0, 0, 0 },
	{ "a??", EX, "a", 0, 0, 0 },
	{ "a?", EX, "a", 0, 0, 1 },
	{ "a{2,3}?", EX, "aaaa", 0, 0, 2 },
	{ "a{2,3}", EX, "aaaa", 0, 0, 3 },
	{ "b+?", EX, "abbb", 0, 1, 2 },
	{ "a.*?c", EX, "abcabc", 0, 0, 3 },
};
static const struct rxs subs[] = {
	{ "(.*?).*", REG_EXTENDED, "abcdef", 0, 2, { 0, 6, 0, 0 } },
};

int main(void)
{
	pc_begin("XSH.regcomp.033");
	pc_test();
	rx_cases(tab, RX_N(tab));
	rx_subs(subs, RX_N(subs));
	pc_pass();
}
