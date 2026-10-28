/*
 * Back-references 1 to 9 and nested group numbering.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.9.3-bre.001
 */
#include "rx.h"

static const struct rxc tab[] = {
	{ "\\(a\\)\\(b\\)\\(c\\)\\(d\\)\\(e\\)\\(f\\)\\(g\\)\\(h\\)\\(i\\)\\9\\8\\7\\6\\5\\4\\3\\2\\1",
	  0, "abcdefghiihgfedcbb", 0, -1, 0 },
	{ "\\(a\\(b\\)\\)\\2\\1", 0, "ababab", 0, -1, 0 },
};
static const struct rxs subs[] = {
	{ "\\(a\\)\\(b\\)\\(c\\)\\(d\\)\\(e\\)\\(f\\)\\(g\\)\\(h\\)\\(i\\)\\9\\8\\7\\6\\5\\4\\3\\2\\1",
	  0, "abcdefghiihgfedcba", 0, 10,
	  { 0, 18, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9 } },
	{ "\\(a\\(b\\)\\)\\2\\1", 0, "abbab", 0, 3, { 0, 5, 0, 2, 1, 2 } },
	{ "\\(a\\(b\\(c\\)\\)\\)\\3\\2\\1", 0, "abccbcabc", 0, 4, { 0, 9, 0, 3, 1, 3, 2, 3 } },
};

int main(void)
{
	pc_begin("XBD.9.3-bre.001");
	pc_test();
	rx_cases(tab, RX_N(tab));
	rx_subs(subs, RX_N(subs));
	pc_pass();
}
