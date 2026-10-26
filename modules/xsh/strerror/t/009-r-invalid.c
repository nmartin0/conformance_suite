/*
 * strerror_r() of an invalid number: an unknown-error message, or EINVAL.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strerror.009
 * MUTANT    strerror.r_invalid_empty
 * MUTANT    strerror.r_minus1
 * VARIANT   strerror.einval_empty
 * VARIANT   strerror.einval_text
 * VARIANT   strerror.r_errno
 */
#include "pc.h"
#include <errno.h>
#include <limits.h>
#include <string.h>

static const int bad[] = { -1, INT_MIN, INT_MAX };
static char buf[65536];

int main(void)
{
	size_t i;
	int r;

	pc_begin("XSH.strerror.009");
	pc_test();
	for (i = 0; i < sizeof bad / sizeof bad[0]; i++) {
		memset(buf, 'X', sizeof buf);
		r = strerror_r(bad[i], buf, sizeof buf);
		if (r == ERANGE)
			pc_unresolved("strerror_r(%d) gave ERANGE with a %zu-byte buffer", bad[i], sizeof buf);
		pc_check(r == 0 || r == EINVAL, "strerror_r(%d) returned %d (%s), expected 0 or EINVAL",
		    bad[i], r, pc_errname(r));
		if (r == 0) {
			pc_check(memchr(buf, '\0', sizeof buf) != NULL, "strerror_r(%d) stored no null byte", bad[i]);
			pc_check(buf[0] != '\0', "strerror_r(%d) returned 0 with an empty string", bad[i]);
		}
	}
	pc_pass();
}
