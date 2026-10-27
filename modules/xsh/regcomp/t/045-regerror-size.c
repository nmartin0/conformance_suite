/*
 * regerror returns the size of the full message and writes it, printable and null-terminated.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.regcomp.045
 */
#include "rx.h"

int main(void)
{
	regex_t re[3];
	int code[3], i;

	pc_begin("XSH.regcomp.045");
	rx_getcodes(re, code);
	pc_test();
	for (i = 0; i < 3; i++) {
		char buf[1024];
		size_t need, n, k;

		need = regerror(code[i], &re[i], NULL, 0);
		pc_check(need >= 1, "code %d: size %zu", code[i], need);
		pc_check(need <= sizeof buf, "code %d: message of %zu bytes", code[i], need);
		memset(buf, 'Z', sizeof buf);
		n = regerror(code[i], &re[i], buf, sizeof buf);
		pc_check(n == need, "code %d: returned %zu, size query gave %zu", code[i], n, need);
		pc_check(strlen(buf) + 1 == need, "code %d: string length %zu, size %zu",
		    code[i], strlen(buf), need);
		for (k = 0; buf[k] != '\0'; k++)
			pc_check(isprint((unsigned char)buf[k]),
			    "code %d: character 0x%02x of the message is not printable",
			    code[i], (unsigned)(unsigned char)buf[k]);
	}
	/* re[0] and re[1] came from failed regcomp() calls: not compiled expressions */
	regfree(&re[2]);
	pc_pass();
}
