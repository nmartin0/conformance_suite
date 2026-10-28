/*
 * Enclosing a BRE in groups does not change what it matches.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.9.3-bre.014
 */
#include "rx.h"

static const char *const str[] = {
	"", "abbb", "xabcabc", "aaa", "zax.yz", "ababc", "ccc", "b", "x.y", "aXa", NULL
};
static const char *const exprs[] = { "ab*", "[ab]*c", "a.c", "a*", "x\\.y", "\\(a\\)*" };

static size_t groups(const char *s)
{
	size_t n = 0;

	while ((s = strstr(s, "\\(")) != NULL) {
		n++;
		s += 2;
	}
	return n;
}

int main(void)
{
	size_t i, k;
	char w[256];
	regex_t re;

	pc_begin("XBD.9.3-bre.014");
	pc_test();
	for (i = 0; i < RX_N(exprs); i++) {
		const char *x = exprs[i];

		snprintf(w, sizeof w, "\\(%s\\)", x);
		rx_same(x, w, 0, str);
		snprintf(w, sizeof w, "\\(\\(%s\\)\\)", x);
		rx_same(x, w, 0, str);
		snprintf(w, sizeof w, "\\(\\(\\(%s\\)\\)\\)", x);
		rx_same(x, w, 0, str);
		w[0] = '\0';
		for (k = 0; k < 20; k++)
			strcat(w, "\\(");
		strcat(w, x);
		for (k = 0; k < 20; k++)
			strcat(w, "\\)");
		rx_same(x, w, 0, str);
		pc_check(regcomp(&re, w, 0) == 0, "regcomp(\"%s\") failed", w);
		pc_check(re.re_nsub == 20 + groups(x), "re_nsub is %zu, expected %zu",
		    (size_t)re.re_nsub, 20 + groups(x));
		regfree(&re);
	}
	pc_pass();
}
