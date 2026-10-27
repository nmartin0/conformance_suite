/*
 * With a buffer size of 0 regerror ignores the buffer and returns the size needed.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.regcomp.047
 */
#include "rx.h"

int main(void)
{
	regex_t re[3];
	int code[3], i;

	pc_begin("XSH.regcomp.047");
	rx_getcodes(re, code);
	pc_test();
	for (i = 0; i < 3; i++) {
		char buf[1024];
		size_t need, n;

		need = regerror(code[i], &re[i], NULL, 0);
		pc_check(need >= 1, "code %d: size %zu", code[i], need);
		memset(buf, 'Z', sizeof buf);
		n = regerror(code[i], &re[i], buf, 0);
		pc_check(n == need, "code %d: with a buffer and size 0 returned %zu, expected %zu",
		    code[i], n, need);
		for (n = 0; n < sizeof buf; n++)
			pc_check(buf[n] == 'Z', "code %d: size 0 but the buffer was written", code[i]);
		n = regerror(code[i], &re[i], buf, sizeof buf);
		pc_check(n == need, "code %d: full copy returned %zu, expected %zu", code[i], n, need);
	}
	/* re[0] and re[1] came from failed regcomp() calls: not compiled expressions */
	regfree(&re[2]);
	pc_pass();
}
