/*
 * regerror truncates a message that does not fit, null-terminates it and still returns the full size.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.regcomp.046
 */
#include "rx.h"

int main(void)
{
	regex_t re[3];
	int code[3], i;

	pc_begin("XSH.regcomp.046");
	rx_getcodes(re, code);
	pc_test();
	for (i = 0; i < 3; i++) {
		char full[1024], buf[1024];
		size_t need, sz, n;

		need = regerror(code[i], &re[i], NULL, 0);
		if (need < 2 || need > sizeof full)
			pc_unresolved("code %d: message size %zu cannot be truncated", code[i], need);
		n = regerror(code[i], &re[i], full, sizeof full);
		pc_check(n == need, "code %d: full copy returned %zu, expected %zu", code[i], n, need);
		for (sz = 1; sz <= need + 1; sz++) {
			memset(buf, 'Z', sizeof buf);
			n = regerror(code[i], &re[i], buf, sz);
			pc_check(n == need, "code %d, size %zu: returned %zu, expected %zu",
			    code[i], sz, n, need);
			if (sz < need) {
				pc_check(buf[sz - 1] == '\0', "code %d, size %zu: not null-terminated",
				    code[i], sz);
				pc_check(memcmp(buf, full, sz - 1) == 0,
				    "code %d, size %zu: not a prefix of the message", code[i], sz);
			} else {
				pc_check(strcmp(buf, full) == 0, "code %d, size %zu: message differs",
				    code[i], sz);
			}
			pc_check(buf[sz] == 'Z', "code %d, size %zu: wrote beyond the buffer size",
			    code[i], sz);
		}
	}
	/* re[0] and re[1] came from failed regcomp() calls: not compiled expressions */
	regfree(&re[2]);
	pc_pass();
}
