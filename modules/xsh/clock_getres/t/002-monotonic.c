/*
 * CLOCK_MONOTONIC is supported: a proper resolution and a proper value.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.clock_getres.002
 * MUTANT    clock_getres.monotonic_einval
 * MUTANT    clock_getres.res_zero
 * MUTANT    clock_getres.res_untouched
 * MUTANT    clock_getres.gettime_bad_nsec
 * VARIANT   clock_getres.coarse
 */
#include "pc.h"
#include <errno.h>
#include <string.h>
#include <time.h>

int main(void)
{
	struct timespec res, tp;
	int r;

	pc_begin("XSH.clock_getres.002");
	memset(&res, 0x5a, sizeof res);
	memset(&tp, 0x5a, sizeof tp);
	pc_test();
	r = clock_getres(CLOCK_MONOTONIC, &res);
	pc_check(r == 0, "clock_getres(CLOCK_MONOTONIC) returned %d (%s)", r, pc_errname(errno));
	pc_check(res.tv_sec >= 0 && res.tv_nsec >= 0 && res.tv_nsec < 1000000000L,
	    "the resolution is not a proper time: %lld s %ld ns", (long long)res.tv_sec, (long)res.tv_nsec);
	pc_check(res.tv_sec > 0 || res.tv_nsec > 0, "the resolution is zero");
	r = clock_gettime(CLOCK_MONOTONIC, &tp);
	pc_check(r == 0, "clock_gettime(CLOCK_MONOTONIC) returned %d (%s)", r, pc_errname(errno));
	pc_check(tp.tv_nsec >= 0 && tp.tv_nsec < 1000000000L,
	    "clock_gettime(CLOCK_MONOTONIC) gave tv_nsec %ld", (long)tp.tv_nsec);
	pc_pass();
}
