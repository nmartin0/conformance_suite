/*
 * gmtime() of the largest time_t value fails with EOVERFLOW.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.gmtime.005
 * MUTANT    gmtime.overflow_wraps
 * MUTANT    gmtime.overflow_errno
 */
#include "pc.h"
#include <errno.h>
#include <limits.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

/* The largest value of time_t (an integer type, XBD <sys/types.h>). */
static time_t time_max(void)
{
	if ((time_t)-1 > 0)
		return (time_t)-1;
	return (time_t)(((uintmax_t)1 << (sizeof(time_t) * CHAR_BIT - 1)) - 1);
}

int main(void)
{
	time_t t;
	struct tm *r;
	int e;

	pc_begin("XSH.gmtime.005");
	t = time_max();
	/* About 31556952 seconds per Gregorian year: is the year beyond int? */
	if ((uintmax_t)t / 31556952u + 1970u - 1900u <= (uintmax_t)INT_MAX)
		pc_unresolved("the year of the largest time_t fits in tm_year here");
	pc_test();
	errno = 0;
	r = gmtime(&t);
	e = errno;
	pc_check(r == NULL, "gmtime(largest time_t) returned a structure (year %d)", r ? r->tm_year : 0);
	pc_check(e == EOVERFLOW, "gmtime(largest time_t) set errno to %s, expected EOVERFLOW", pc_errname(e));
	pc_pass();
}
