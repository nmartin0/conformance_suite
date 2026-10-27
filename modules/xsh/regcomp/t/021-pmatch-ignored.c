/*
 * With nmatch 0, or REG_NOSUB at compile time, pmatch is ignored.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.regcomp.021
 */
#include "rx.h"

int main(void)
{
	regex_t re;
	regmatch_t m[3];
	int k, r;

	pc_begin("XSH.regcomp.021");
	pc_test();
	for (k = 0; k < 3; k++)
		m[k].rm_so = m[k].rm_eo = RX_SENT;

	r = regcomp(&re, "b", 0);
	pc_check(r == 0, "regcomp returned %d", r);
	pc_check(regexec(&re, "abc", 0, m, 0) == 0, "nmatch 0: no match reported");
	pc_check(regexec(&re, "abc", 0, NULL, 0) == 0, "nmatch 0, null pmatch: no match reported");
	pc_check(regexec(&re, "xyz", 0, NULL, 0) == REG_NOMATCH, "nmatch 0: spurious match");
	for (k = 0; k < 3; k++)
		pc_check(m[k].rm_so == RX_SENT && m[k].rm_eo == RX_SENT,
		    "nmatch 0: pmatch[%d] was written", k);
	regfree(&re);

	r = regcomp(&re, "\\(b\\)", REG_NOSUB);
	pc_check(r == 0, "regcomp (REG_NOSUB) returned %d", r);
	pc_check(regexec(&re, "abc", 3, NULL, 0) == 0, "REG_NOSUB, null pmatch: no match reported");
	pc_check(regexec(&re, "abc", 3, m, 0) == 0, "REG_NOSUB: no match reported");
	pc_check(regexec(&re, "xyz", 3, NULL, 0) == REG_NOMATCH, "REG_NOSUB: spurious match");
	for (k = 0; k < 3; k++)
		pc_check(m[k].rm_so == RX_SENT && m[k].rm_eo == RX_SENT,
		    "REG_NOSUB: pmatch[%d] was written", k);
	regfree(&re);
	pc_pass();
}
