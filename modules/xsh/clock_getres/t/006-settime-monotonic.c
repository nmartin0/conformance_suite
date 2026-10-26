/*
 * CLOCK_MONOTONIC cannot be set.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.clock_getres.006
 * MUTANT    clock_getres.settime_monotonic_ok
 * MUTANT    clock_getres.settime_monotonic_enotsup
 * VARIANT   clock_getres.settime_eperm
 */
#include "pc.h"
#include <errno.h>
#include <time.h>

int main(void)
{
	struct timespec now;
	int r, e;

	pc_begin("XSH.clock_getres.006");
	pc_check(clock_gettime(CLOCK_MONOTONIC, &now) == 0, "clock_gettime(CLOCK_MONOTONIC) failed: %s", pc_errname(errno));
	pc_test();
	/* Its own current value: a system that wrongly accepts it hardly moves it. */
	errno = 0;
	r = clock_settime(CLOCK_MONOTONIC, &now);
	e = errno;
	pc_check(r == -1, "clock_settime(CLOCK_MONOTONIC) returned %d", r);
	pc_check(e == EINVAL || e == EPERM, "clock_settime(CLOCK_MONOTONIC) failed with %s, expected EINVAL (or EPERM)",
	    pc_errname(e));
	if (e == EPERM)
		pc_note("EPERM: this process may not set clocks; EINVAL applies as well (XSH 2.3)");
	pc_pass();
}
