/*
 * strerror() of an invalid number: a string, with errno 0 or EINVAL.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strerror.005
 * MUTANT    strerror.null_invalid
 * MUTANT    strerror.invalid_erange
 * VARIANT   strerror.einval_empty
 * VARIANT   strerror.einval_text
 * VARIANT   strerror.unknown_empty
 */
#include "pc.h"
#include <errno.h>
#include <limits.h>
#include <string.h>

static const int bad[] = { -1, INT_MIN, INT_MAX };

int main(void)
{
	size_t i;
	const char *s;
	int e;

	pc_begin("XSH.strerror.005");
	pc_test();
	for (i = 0; i < sizeof bad / sizeof bad[0]; i++) {
		errno = 0;
		s = strerror(bad[i]);
		e = errno;
		pc_check(s != NULL, "strerror(%d) returned a null pointer", bad[i]);
		pc_check(e == 0 || e == EINVAL, "strerror(%d) set errno to %s, expected 0 or EINVAL",
		    bad[i], pc_errname(e));
	}
	pc_pass();
}
