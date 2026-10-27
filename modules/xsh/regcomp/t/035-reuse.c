/*
 * A compiled expression can be used for many strings and flags; a failed match returns REG_NOMATCH.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.regcomp.035
 */
#include "rx.h"

int main(void)
{
	regex_t a, b;
	regmatch_t m[2];
	int round, r;

	pc_begin("XSH.regcomp.035");
	pc_test();
	r = regcomp(&a, "ab*c", 0);
	pc_check(r == 0, "regcomp returned %d", r);
	r = regcomp(&b, "x\\(y\\)z", 0);
	pc_check(r == 0, "regcomp returned %d", r);
	pc_check(REG_NOMATCH != 0, "REG_NOMATCH is zero");
	for (round = 0; round < 3; round++) {
		r = regexec(&a, "ac", 1, m, 0);
		pc_check(r == 0 && m[0].rm_so == 0 && m[0].rm_eo == 2, "round %d: ac", round);
		r = regexec(&b, "wxyzw", 2, m, 0);
		pc_check(r == 0 && m[0].rm_so == 1 && m[0].rm_eo == 4 &&
		    m[1].rm_so == 2 && m[1].rm_eo == 3, "round %d: wxyzw", round);
		r = regexec(&a, "abbc", 1, m, 0);
		pc_check(r == 0 && m[0].rm_so == 0 && m[0].rm_eo == 4, "round %d: abbc", round);
		r = regexec(&a, "xx", 1, m, 0);
		pc_check(r == REG_NOMATCH, "round %d: xx returned %d, expected REG_NOMATCH", round, r);
		r = regexec(&a, "zabcz", 1, m, REG_NOTBOL | REG_NOTEOL);
		pc_check(r == 0 && m[0].rm_so == 1 && m[0].rm_eo == 4, "round %d: zabcz", round);
		r = regexec(&b, "xz", 2, m, 0);
		pc_check(r == REG_NOMATCH, "round %d: xz returned %d, expected REG_NOMATCH", round, r);
	}
	regfree(&a);
	regfree(&b);
	pc_pass();
}
