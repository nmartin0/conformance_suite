/*
 * A period matches every character but NUL.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.regcomp.011
 */
#include "rx.h"

int main(void)
{
	regex_t bre, ere;
	char s[4];
	int c, r;

	pc_begin("XSH.regcomp.011");
	pc_test();
	pc_check(regcomp(&bre, "x.y", 0) == 0, "regcomp (BRE) failed");
	pc_check(regcomp(&ere, "x.y", REG_EXTENDED) == 0, "regcomp (ERE) failed");
	for (c = 1; c <= 255; c++) {
		regmatch_t m[1];

		s[0] = 'x';
		s[1] = (char)c;
		s[2] = 'y';
		s[3] = '\0';
		r = regexec(&bre, s, 1, m, 0);
		pc_check(r == 0 && m[0].rm_so == 0 && m[0].rm_eo == 3,
		    "BRE period did not match the character 0x%02x", c);
		r = regexec(&ere, s, 1, m, 0);
		pc_check(r == 0 && m[0].rm_so == 0 && m[0].rm_eo == 3,
		    "ERE period did not match the character 0x%02x", c);
	}
	pc_check(regexec(&bre, "xy", 0, NULL, 0) == REG_NOMATCH,
	    "a period matched nothing");
	regfree(&bre);
	regfree(&ere);
	pc_pass();
}
