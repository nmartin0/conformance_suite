/*
 * re_nsub counts the parenthesized subexpressions.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.regcomp.001
 */
#include "rx.h"

static const struct { const char *pat; int cf; size_t n; } tab[] = {
	{ "abc", 0, 0 },
	{ "\\(a\\)", 0, 1 },
	{ "\\(a\\)\\(b\\)", 0, 2 },
	{ "\\(a\\(b\\)\\)", 0, 2 },
	{ "\\(a\\)*\\(b\\)", 0, 2 },
	{ "(a)", 0, 0 },
	{ "abc", REG_EXTENDED, 0 },
	{ "(a)", REG_EXTENDED, 1 },
	{ "(a)(b)", REG_EXTENDED, 2 },
	{ "((a)b)", REG_EXTENDED, 2 },
	{ "(a|b)(c|d)", REG_EXTENDED, 2 },
	{ "(a)*", REG_EXTENDED, 1 },
	{ "(a)+(b)?(c){2}", REG_EXTENDED, 3 },
	{ "a\\(b", REG_EXTENDED, 0 },
	{ "[(]", REG_EXTENDED, 0 },
	{ "((((((((((a))))))))))", REG_EXTENDED, 10 },
};

int main(void)
{
	size_t i;

	pc_begin("XSH.regcomp.001");
	pc_test();
	for (i = 0; i < RX_N(tab); i++) {
		regex_t re;
		int r = regcomp(&re, tab[i].pat, tab[i].cf);

		pc_check(r == 0, "regcomp(\"%s\") returned %d", tab[i].pat, r);
		pc_check(re.re_nsub == tab[i].n, "\"%s\": re_nsub is %zu, expected %zu",
		    tab[i].pat, (size_t)re.re_nsub, tab[i].n);
		regfree(&re);
	}
	pc_pass();
}
