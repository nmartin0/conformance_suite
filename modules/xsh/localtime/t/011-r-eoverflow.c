/*
 * localtime_r() of the largest time_t value fails with EOVERFLOW.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.localtime.011
 * MUTANT    localtime.r_overflow_wraps
 * MUTANT    localtime.overflow_errno
 */
#include "pc.h"
#include <errno.h>
#include <limits.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

static time_t time_max(void)
{
	if ((time_t)-1 > 0)
		return (time_t)-1;
	return (time_t)(((uintmax_t)1 << (sizeof(time_t) * CHAR_BIT - 1)) - 1);
}

int main(void)
{
	time_t t;
	struct tm buf, *r;
	int e;

	pc_begin("XSH.localtime.011");
	if (setenv("TZ", "PCT3", 1) != 0)
		pc_unresolved("setenv(TZ) failed");
	tzset();
	t = time_max();
	if ((uintmax_t)t / 31556952u + 1970u - 1900u <= (uintmax_t)INT_MAX)
		pc_unresolved("the year of the largest time_t fits in tm_year here");
	pc_test();
	errno = 0;
	r = localtime_r(&t, &buf);
	e = errno;
	pc_check(r == NULL, "localtime_r(largest time_t) returned %p (year %d)", (void *)r, r ? r->tm_year : 0);
	pc_check(e == EOVERFLOW, "localtime_r(largest time_t) set errno to %s, expected EOVERFLOW", pc_errname(e));
	pc_pass();
}
