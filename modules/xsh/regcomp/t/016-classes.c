/*
 * The twelve character classes in the POSIX locale.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.regcomp.016
 */
#include "rx.h"

static const struct { const char *name; int (*fn)(int); const char *yes, *no; } cls[] = {
	{ "alnum", isalnum, "a9Z", "_ -" },
	{ "alpha", isalpha, "aZ", "1 _" },
	{ "blank", isblank, " \t", "\na" },
	{ "cntrl", iscntrl, "\001\177\n", "a " },
	{ "digit", isdigit, "09", "ax" },
	{ "graph", isgraph, "a!~9", " \t" },
	{ "lower", islower, "az", "A1" },
	{ "print", isprint, "a ~", "\001\177" },
	{ "punct", ispunct, "!_~", "a1 " },
	{ "space", isspace, " \t\n\r\f\v", "a_" },
	{ "upper", isupper, "AZ", "a1" },
	{ "xdigit", isxdigit, "09afAF", "gG" },
};

int main(void)
{
	size_t i;
	int c;

	pc_begin("XSH.regcomp.016");
	pc_test();
	for (i = 0; i < RX_N(cls); i++) {
		char pat[32], npat[32], s[2];
		regex_t re, nre;
		int r;

		snprintf(pat, sizeof pat, "[[:%s:]]", cls[i].name);
		snprintf(npat, sizeof npat, "[^[:%s:]]", cls[i].name);
		r = regcomp(&re, pat, 0);
		pc_check(r == 0, "regcomp(\"%s\") returned %d", pat, r);
		r = regcomp(&nre, npat, REG_EXTENDED);
		pc_check(r == 0, "regcomp(\"%s\") returned %d", npat, r);
		s[1] = '\0';
		for (c = 1; c < 128; c++) {
			int want = cls[i].fn(c) != 0;

			s[0] = (char)c;
			r = regexec(&re, s, 0, NULL, 0);
			pc_check((r == 0) == want, "\"%s\" on 0x%02x: match %d, expected %d",
			    pat, c, r == 0, want);
			r = regexec(&nre, s, 0, NULL, 0);
			pc_check((r == 0) == !want, "\"%s\" on 0x%02x: match %d, expected %d",
			    npat, c, r == 0, !want);
		}
		regfree(&re);
		regfree(&nre);
		rx_set(pat, 0, cls[i].yes, cls[i].no);
	}
	rx_set("[[:alpha:][:digit:]_]", 0, "a5_Z", "-. ");
	pc_pass();
}
