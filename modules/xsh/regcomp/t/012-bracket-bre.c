/*
 * BRE bracket expressions: lists, non-matching lists, closing bracket first, specials inert.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.regcomp.012
 */
#include "rx.h"

static const struct rxc tab[] = {
	{ "a[^b]c", 0, "abc adc", 0, 4, 7 },
	{ "[ab]", 0, "ab", 0, 0, 1 },
	{ "[abc]", 0, "xyz", 0, -1, 0 },
	{ "[abc]", 0, "xxbxx", 0, 2, 3 },
	{ "[^abc]", 0, "abcd", 0, 3, 4 },
	{ "[^abc]", 0, "abc", 0, -1, 0 },
};

int main(void)
{
	pc_begin("XSH.regcomp.012");
	pc_test();
	rx_cases(tab, RX_N(tab));
	rx_set("[abc]", 0, "abc", "dxz.");
	rx_set("[^abc]", 0, "dxz.", "abc");
	rx_set("[]a]", 0, "]a", "b");
	rx_set("[^]a]", 0, "bz", "]a");
	rx_set("[.*]", 0, ".*", "ab");
	rx_set("[\\]", 0, "\\", "n.");
	rx_set("[\\n]", 0, "\\n", "x\n");
	rx_set("[[]", 0, "[", "a");
	rx_set("[a^]", 0, "a^", "b");
	pc_pass();
}
