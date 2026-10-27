/*
 * REG_NOSUB: regexec reports only success or failure.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.regcomp.028
 */
#include "rx.h"

static const struct { const char *pat; int cf; const char *str; int match; } tab[] = {
	{ "b+(bc)", REG_EXTENDED, "acabbbcde", 1 },
	{ "b+(bc)", REG_EXTENDED, "xxx", 0 },
	{ "\\(ab\\)\\{4,\\}", 0, "abababccccccd", 0 },
	{ "\\(ab\\)\\{3,\\}", 0, "abababccccccd", 1 },
	{ "^abcdef$", 0, "abcdef", 1 },
	{ "^abcdef$", 0, "abcdefg", 0 },
	{ "a.c", 0, "xabcx", 1 },
	{ "(wee|week)(knights|night)", REG_EXTENDED, "weeknights", 1 },
};

int main(void)
{
	size_t i;

	pc_begin("XSH.regcomp.028");
	pc_test();
	for (i = 0; i < RX_N(tab); i++) {
		regex_t re;
		regmatch_t m[2];
		int r;

		r = regcomp(&re, tab[i].pat, tab[i].cf | REG_NOSUB);
		pc_check(r == 0, "case %zu: regcomp returned %d", i, r);
		r = regexec(&re, tab[i].str, 0, NULL, 0);
		pc_check(tab[i].match ? r == 0 : r == REG_NOMATCH,
		    "case %zu: \"%s\" on \"%s\": regexec returned %d", i, tab[i].pat,
		    tab[i].str, r);
		r = regexec(&re, tab[i].str, 2, m, 0);
		pc_check(tab[i].match ? r == 0 : r == REG_NOMATCH,
		    "case %zu: with nmatch 2: regexec returned %d", i, r);
		regfree(&re);
	}
	pc_pass();
}
