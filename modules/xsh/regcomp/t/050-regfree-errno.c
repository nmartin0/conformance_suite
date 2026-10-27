/*
 * regfree leaves errno alone and the object can be compiled into again.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.regcomp.050
 */
#include "rx.h"

#include <errno.h>

int main(void)
{
	regex_t re;
	regmatch_t m[1];
	int r, k;
	static const int cfs[] = { 0, REG_EXTENDED, REG_NOSUB, REG_ICASE | REG_NEWLINE };

	pc_begin("XSH.regcomp.050");
	pc_test();
	for (k = 0; k < 4; k++) {
		r = regcomp(&re, (cfs[k] & REG_EXTENDED) ? "a(b)" : "a\\(b\\)", cfs[k]);
		pc_check(r == 0, "regcomp returned %d", r);
		errno = EDOM;
		regfree(&re);
		pc_check(errno == EDOM, "regfree changed errno to %d", errno);
	}
	r = regcomp(&re, "x", 0);
	pc_check(r == 0, "regcomp returned %d", r);
	regfree(&re);
	r = regcomp(&re, "y+", REG_EXTENDED);
	pc_check(r == 0, "regcomp after regfree returned %d", r);
	r = regexec(&re, "ayyb", 1, m, 0);
	pc_check(r == 0 && m[0].rm_so == 1 && m[0].rm_eo == 3, "the recompiled expression did not match");
	regfree(&re);
	pc_pass();
}
