/*
 * A parenthesised single-character ERE matches what the bare one does.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.9.4-ere.001
 */
#include "rx.h"

static const char *const single[] = { "a", ".", "[ab]", "\\.", "[^a]", "\\*", "[[:digit:]]", "x" };
static const char *const suffix[] = { "", "*", "+", "?", "{2}", "{1,2}" };
static const char *const str[] = {
	"", "a", "ab", "xaaby", "bbb.a*", "a.b", "1a2", "zzz", "aaaa", "..", "cab", NULL
};

int main(void)
{
	size_t i, k;
	char a[64], b[64];

	pc_begin("XBD.9.4-ere.001");
	pc_test();
	for (i = 0; i < RX_N(single); i++)
		for (k = 0; k < RX_N(suffix); k++) {
			snprintf(a, sizeof a, "%s%s", single[i], suffix[k]);
			snprintf(b, sizeof b, "(%s)%s", single[i], suffix[k]);
			rx_same(a, b, REG_EXTENDED, str);
		}
	for (k = 0; k < RX_N(suffix); k++) {
		snprintf(a, sizeof a, "[ab]%s", suffix[k]);
		snprintf(b, sizeof b, "(a|b)%s", suffix[k]);
		rx_same(a, b, REG_EXTENDED, str);
		snprintf(a, sizeof a, "[abc]%s", suffix[k]);
		snprintf(b, sizeof b, "(a|b|c)%s", suffix[k]);
		rx_same(a, b, REG_EXTENDED, str);
	}
	pc_pass();
}
