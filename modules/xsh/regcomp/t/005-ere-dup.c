/*
 * ERE duplication symbols and interval expressions.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.regcomp.005
 */
#include "rx.h"

#define S "abababccccccd"
static const struct rxc tab[] = {
	{ "b+(bc)", REG_EXTENDED, "acabbbcde", 0, 3, 7 },
	{ "[ab]+", REG_EXTENDED, "xabbay", 0, 1, 5 },
	{ "b*c", REG_EXTENDED, "cabbbcde", 0, 0, 1 },
	{ "b*cd", REG_EXTENDED, "cabbbcdebbbbbbcdbc", 0, 2, 7 },
	{ "b?c", REG_EXTENDED, "acabbbcde", 0, 1, 2 },
	{ "c{3}", REG_EXTENDED, S, 0, 6, 9 },
	{ "(ab){2,}", REG_EXTENDED, S, 0, 0, 6 },
	{ "c{1,3}d", REG_EXTENDED, S, 0, 9, 13 },
	{ "(ab)+", REG_EXTENDED, "xababab", 0, 1, 7 },
	{ "a{2}", REG_EXTENDED, "aaa", 0, 0, 2 },
	{ "a?", REG_EXTENDED, "b", 0, 0, 0 },
	{ "a+", REG_EXTENDED, "b", 0, -1, 0 },
	{ "cd", REG_EXTENDED, "abcdefabcdef", 0, 2, 4 },
	{ "(cd)", REG_EXTENDED, "abcdefabcdef", 0, 2, 4 },
	{ "[ab]*", REG_EXTENDED, "ab", 0, 0, 2 },
	{ "a{0,2}b", REG_EXTENDED, "aaab", 0, 1, 4 },
};

int main(void)
{
	pc_begin("XSH.regcomp.005");
	pc_test();
	rx_cases(tab, RX_N(tab));
	pc_pass();
}
