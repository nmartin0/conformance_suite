/*
 * Expressions of up to 256 bytes are supported.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.regcomp.036
 */
#include "rx.h"

int main(void)
{
	static char pat[300], str[400];
	regex_t re;
	regmatch_t m[1];
	int i, r;

	pc_begin("XSH.regcomp.036");
	pc_test();

	/* 256 literal characters */
	memset(pat, 'x', 256);
	pat[256] = '\0';
	memset(str, 'x', 300);
	str[300] = '\0';
	r = regcomp(&re, pat, 0);
	pc_check(r == 0, "256-byte BRE: regcomp returned %d", r);
	r = regexec(&re, str, 1, m, 0);
	pc_check(r == 0 && m[0].rm_so == 0 && m[0].rm_eo == 256, "256-byte BRE: wrong match");
	regfree(&re);
	r = regcomp(&re, pat, REG_EXTENDED);
	pc_check(r == 0, "256-byte ERE: regcomp returned %d", r);
	r = regexec(&re, str, 1, m, 0);
	pc_check(r == 0 && m[0].rm_so == 0 && m[0].rm_eo == 256, "256-byte ERE: wrong match");
	regfree(&re);

	/* 51 groups of 5 bytes and one more byte */
	pat[0] = '\0';
	for (i = 0; i < 51; i++)
		strcat(pat, "\\(a\\)");
	strcat(pat, "a");
	pc_check(strlen(pat) == 256, "setup: pattern length %zu", strlen(pat));
	memset(str, 'a', 52);
	str[52] = '\0';
	r = regcomp(&re, pat, 0);
	pc_check(r == 0, "256-byte BRE with groups: regcomp returned %d", r);
	pc_check(re.re_nsub == 51, "re_nsub is %zu, expected 51", (size_t)re.re_nsub);
	r = regexec(&re, str, 1, m, 0);
	pc_check(r == 0 && m[0].rm_so == 0 && m[0].rm_eo == 52, "256-byte BRE with groups: wrong match");
	regfree(&re);

	/* 85 groups of 3 bytes and one more byte */
	pat[0] = '\0';
	for (i = 0; i < 85; i++)
		strcat(pat, "(a)");
	strcat(pat, "a");
	pc_check(strlen(pat) == 256, "setup: pattern length %zu", strlen(pat));
	memset(str, 'a', 86);
	str[86] = '\0';
	r = regcomp(&re, pat, REG_EXTENDED);
	pc_check(r == 0, "256-byte ERE with groups: regcomp returned %d", r);
	pc_check(re.re_nsub == 85, "re_nsub is %zu, expected 85", (size_t)re.re_nsub);
	r = regexec(&re, str, 1, m, 0);
	pc_check(r == 0 && m[0].rm_so == 0 && m[0].rm_eo == 86, "256-byte ERE with groups: wrong match");
	regfree(&re);
	pc_pass();
}
