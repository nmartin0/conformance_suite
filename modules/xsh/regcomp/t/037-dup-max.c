/*
 * Interval counts up to 255 are accepted.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.regcomp.037
 */
#include "rx.h"

#include <limits.h>

int main(void)
{
	static char str[400];
	regex_t re;
	regmatch_t m[1];
	int r;

	pc_begin("XSH.regcomp.037");
	pc_test();
#ifdef RE_DUP_MAX
	pc_check(RE_DUP_MAX >= 255, "RE_DUP_MAX is %ld", (long)RE_DUP_MAX);
#endif
	memset(str, 'a', 300);
	str[300] = '\0';
	r = regcomp(&re, "a\\{255\\}", 0);
	pc_check(r == 0, "BRE a\\{255\\}: regcomp returned %d", r);
	r = regexec(&re, str, 1, m, 0);
	pc_check(r == 0 && m[0].rm_so == 0 && m[0].rm_eo == 255, "BRE a\\{255\\}: wrong match");
	str[254] = '\0';
	r = regexec(&re, str, 0, NULL, 0);
	pc_check(r == REG_NOMATCH, "BRE a\\{255\\} matched 254 characters");
	regfree(&re);
	memset(str, 'a', 300);
	str[300] = '\0';
	r = regcomp(&re, "a{0,255}", REG_EXTENDED);
	pc_check(r == 0, "ERE a{0,255}: regcomp returned %d", r);
	r = regexec(&re, str, 1, m, 0);
	pc_check(r == 0 && m[0].rm_so == 0 && m[0].rm_eo == 255, "ERE a{0,255}: wrong match");
	regfree(&re);
	r = regcomp(&re, "a{255}", REG_EXTENDED);
	pc_check(r == 0, "ERE a{255}: regcomp returned %d", r);
	r = regexec(&re, str, 1, m, 0);
	pc_check(r == 0 && m[0].rm_so == 0 && m[0].rm_eo == 255, "ERE a{255}: wrong match");
	regfree(&re);
	pc_pass();
}
