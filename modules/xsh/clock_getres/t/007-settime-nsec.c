/*
 * clock_settime() rejects a nanosecond value out of range.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.clock_getres.007
 * MUTANT    clock_getres.settime_nsec_ok
 * MUTANT    clock_getres.settime_nsec_neg_ok
 * VARIANT   clock_getres.settime_eperm
 */
#include "pc.h"
#include <errno.h>
#include <time.h>

static void expect_einval(const struct timespec *tp, const char *what)
{
	int r, e;

	errno = 0;
	r = clock_settime(CLOCK_REALTIME, tp);
	e = errno;
	pc_check(r == -1, "clock_settime(CLOCK_REALTIME) with %s returned %d", what, r);
	pc_check(e == EINVAL || e == EPERM, "clock_settime(CLOCK_REALTIME) with %s failed with %s, expected EINVAL (or EPERM)",
	    what, pc_errname(e));
}

int main(void)
{
	struct timespec now, a, b;

	pc_begin("XSH.clock_getres.007");
	pc_check(clock_gettime(CLOCK_REALTIME, &now) == 0, "clock_gettime(CLOCK_REALTIME) failed: %s", pc_errname(errno));
	/* Carried into seconds, either value is about now. */
	a.tv_sec = now.tv_sec - 1;
	a.tv_nsec = 1000000000L;
	b.tv_sec = now.tv_sec + 1;
	b.tv_nsec = -1;
	pc_test();
	expect_einval(&a, "tv_nsec 1000000000");
	expect_einval(&b, "tv_nsec -1");
	pc_pass();
}
