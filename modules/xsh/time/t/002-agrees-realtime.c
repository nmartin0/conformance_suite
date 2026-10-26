/*
 * time() and CLOCK_REALTIME tell the same time.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.time.002
 * MUTANT    time.frozen
 * MUTANT    time.offset
 * VARIANT   time.lags
 * VARIANT   clock_getres.coarse
 */
#include "pc.h"
#include <errno.h>
#include <poll.h>
#include <time.h>

static long long ns_of(clockid_t id)
{
	struct timespec t;

	if (clock_gettime(id, &t) != 0)
		pc_unresolved("clock_gettime(%d) failed: %s", (int)id, pc_errname(errno));
	return (long long)t.tv_sec * 1000000000LL + t.tv_nsec;
}

static void agree(const char *when)
{
	struct timespec a, b;
	time_t v;

	pc_check(clock_gettime(CLOCK_REALTIME, &a) == 0, "clock_gettime(CLOCK_REALTIME) failed: %s", pc_errname(errno));
	v = time(NULL);
	pc_check(clock_gettime(CLOCK_REALTIME, &b) == 0, "clock_gettime(CLOCK_REALTIME) failed: %s", pc_errname(errno));
	pc_check(v != (time_t)-1, "time(NULL) returned (time_t)-1");
	pc_check(v >= a.tv_sec - 1 && v <= b.tv_sec + 1,
	    "%s, time() returned %lld while CLOCK_REALTIME read %lld.%09ld s and then %lld.%09ld s", when,
	    (long long)v, (long long)a.tv_sec, (long)a.tv_nsec, (long long)b.tv_sec, (long)b.tv_nsec);
}

int main(void)
{
	long long r0, m0, dr, dm, i;

	pc_begin("XSH.time.002");
	pc_test();
	m0 = ns_of(CLOCK_MONOTONIC);
	r0 = ns_of(CLOCK_REALTIME);
	agree("at first");
	/* 2.2 s by CLOCK_MONOTONIC, waiting with poll() */
	for (i = 0; ns_of(CLOCK_MONOTONIC) - m0 < 2200000000LL && i < 1000; i++)
		(void)poll(NULL, 0, 50);
	dm = ns_of(CLOCK_MONOTONIC) - m0;
	dr = ns_of(CLOCK_REALTIME) - r0;
	if (dr - dm > 100000000LL || dm - dr > 100000000LL)
		pc_unresolved("CLOCK_REALTIME moved %lld ms while CLOCK_MONOTONIC moved %lld ms (the clock was set)",
		    dr / 1000000LL, dm / 1000000LL);
	agree("2.2 s later");
	pc_pass();
}
