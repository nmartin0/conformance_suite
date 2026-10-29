/*
 * A repeated subexpression takes no needless null iteration.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.9.3-bre.003
 */
#include "rx.h"

static const struct rxs subs[] = {
	{ "\\(a*\\)*", 0, "b", 0, 2, { 0, 0, 0, 0 } },
	{ "\\(ab*\\)*", 0, "abbab", 0, 2, { 0, 5, 3, 5 } },
};
/*
 * Cases where a repeated group could also make one more, empty, iteration after its last
 * non-empty one: the text does not say whether it does (Q-207), so group 1 may be either
 * the last non-empty match or the empty string right after it.
 */
struct amb {
	const char *pat;
	int cf;
	const char *str;
	int so, eo;	/* whole match */
	int g1so, g1eo;	/* group 1, no extra empty iteration */
	int g2so, g2eo;	/* group 1, extra empty iteration */
};

static void amb_cases(const struct amb *t, size_t n)
{
	size_t i;

	for (i = 0; i < n; i++) {
		regex_t re;
		regmatch_t m[2];
		int r;

		r = regcomp(&re, t[i].pat, t[i].cf);
		pc_check(r == 0, "case %zu: regcomp(\"%s\") returned %d", i, t[i].pat, r);
		r = regexec(&re, t[i].str, 2, m, 0);
		pc_check(r == 0, "case %zu: \"%s\" on \"%s\" should match, got %d", i, t[i].pat, t[i].str, r);
		pc_check(m[0].rm_so == t[i].so && m[0].rm_eo == t[i].eo,
		    "case %zu: \"%s\" on \"%s\" matched [%d,%d], expected [%d,%d]", i, t[i].pat, t[i].str,
		    (int)m[0].rm_so, (int)m[0].rm_eo, t[i].so, t[i].eo);
		pc_check((m[1].rm_so == t[i].g1so && m[1].rm_eo == t[i].g1eo) ||
		    (m[1].rm_so == t[i].g2so && m[1].rm_eo == t[i].g2eo),
		    "case %zu: \"%s\" on \"%s\": pmatch[1] is [%d,%d], expected [%d,%d] or [%d,%d]", i, t[i].pat,
		    t[i].str, (int)m[1].rm_so, (int)m[1].rm_eo, t[i].g1so, t[i].g1eo, t[i].g2so, t[i].g2eo);
		regfree(&re);
	}
}
static const struct amb ambs[] = {
	{ "\\(a*\\)*", 0, "aab", 0, 2, 0, 2, 2, 2 },
	{ "\\(a*\\)*c", 0, "aac", 0, 3, 0, 2, 2, 2 },
	{ "\\(a*b*\\)*", 0, "aabb", 0, 4, 0, 4, 4, 4 },
};
static const struct rxc tab[] = {
	{ "\\(x*\\)\\{2\\}", 0, "x", 0, 0, 1 },
	{ "\\(x*\\)\\{2\\}", 0, "", 0, 0, 0 },
};

int main(void)
{
	pc_begin("XBD.9.3-bre.003");
	pc_test();
	rx_subs(subs, RX_N(subs));
	amb_cases(ambs, RX_N(ambs));
	rx_cases(tab, RX_N(tab));
	pc_pass();
}
