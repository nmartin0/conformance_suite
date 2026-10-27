/*
 * The message regerror produces for a real error code is not empty.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.regcomp.049
 */
#include "rx.h"

int main(void)
{
	regex_t re[3];
	int code[3], i;

	pc_begin("XSH.regcomp.049");
	rx_getcodes(re, code);
	pc_test();
	for (i = 0; i < 3; i++) {
		char buf[1024];
		size_t n;

		n = regerror(code[i], &re[i], buf, sizeof buf);
		pc_check(n >= 2 && n <= sizeof buf && buf[0] != '\0',
		    "code %d: the message is empty", code[i]);
		n = regerror(code[i], NULL, buf, sizeof buf);
		pc_check(n >= 2 && n <= sizeof buf && buf[0] != '\0',
		    "code %d: the message is empty with a null preg", code[i]);
	}
	/* re[0] and re[1] came from failed regcomp() calls: not compiled expressions */
	regfree(&re[2]);
	pc_pass();
}
