/*
 * confstr() truncates the value to len - 1 bytes and a null byte, and
 * writes nothing past len bytes.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.confstr.003
 * MUTANT    confstr.no_terminator
 * MUTANT    confstr.ignores_len
 * MUTANT    confstr.returns_copied
 */
#include "pc.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(void)
{
	char full[8192], buf[8192 + 16];
	size_t n, len, r, k;

	pc_begin("XSH.confstr.003");
	n = confstr(_CS_PATH, full, sizeof full);
	pc_check(n > 1 && n <= sizeof full, "confstr(_CS_PATH) returned %zu", n);
	pc_test();
	for (len = 1; len < n; len++) {
		memset(buf, '#', sizeof buf);
		r = confstr(_CS_PATH, buf, len);
		pc_check(r == n, "confstr(_CS_PATH, buf, %zu) returned %zu, expected %zu (the full size)", len, r, n);
		pc_check(memcmp(buf, full, len - 1) == 0, "confstr(_CS_PATH, buf, %zu) stored other bytes than the value's "
			 "first %zu", len, len - 1);
		pc_check(buf[len - 1] == '\0', "confstr(_CS_PATH, buf, %zu) did not end the truncated value with a null "
			 "byte at buf[%zu]", len, len - 1);
		for (k = len; k < len + 16; k++)
			pc_check(buf[k] == '#', "confstr(_CS_PATH, buf, %zu) wrote buf[%zu], past the buffer", len, k);
	}
	pc_pass();
}
