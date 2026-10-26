/*
 * strerror_l() of an invalid number still returns a string.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strerror.012
 * MUTANT    strerror.l_invalid_null
 * MUTANT    strerror.l_invalid_erange
 * VARIANT   strerror.einval_empty
 * VARIANT   strerror.einval_text
 * VARIANT   strerror.unknown_empty
 */
#include "pc.h"
#include <errno.h>
#include <limits.h>
#include <locale.h>
#include <string.h>

static const int bad[] = { -1, INT_MIN, INT_MAX };

int main(void)
{
	locale_t loc;
	const char *s;
	size_t i;
	int e;

	pc_begin("XSH.strerror.012");
	loc = newlocale(LC_ALL_MASK, "POSIX", (locale_t)0);
	pc_check(loc != (locale_t)0, "newlocale(POSIX) failed: %s", pc_errname(errno));
	pc_test();
	for (i = 0; i < sizeof bad / sizeof bad[0]; i++) {
		errno = 0;
		s = strerror_l(bad[i], loc);
		e = errno;
		pc_check(s != NULL, "strerror_l(%d) returned a null pointer (errno %s)", bad[i], pc_errname(e));
		pc_check(e == 0 || e == EINVAL, "strerror_l(%d) set errno to %s, expected 0 or EINVAL",
		    bad[i], pc_errname(e));
	}
	pc_pass();
}
