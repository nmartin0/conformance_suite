/*
 * CLOCK_PROCESS_CPUTIME_ID measures the process's execution time.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.clock_getres.008
 * MUTANT    clock_getres.cputime_einval
 * MUTANT    clock_getres.cputime_is_wall
 * MUTANT    clock_getres.cputime_frozen
 */
#include "pc.h"
#include <errno.h>
#include <time.h>

static long long ns_of(clockid_t id, const char *name)
{
	struct timespec t;

	pc_check(clock_gettime(id, &t) == 0, "clock_gettime(%s) failed: %s", name, pc_errname(errno));
	pc_check(t.tv_nsec >= 0 && t.tv_nsec < 1000000000L, "clock_gettime(%s) gave tv_nsec %ld", name, (long)t.tv_nsec);
	return (long long)t.tv_sec * 1000000000LL + t.tv_nsec;
}

static long long wall(void)
{
	struct timespec t;

	if (clock_gettime(CLOCK_MONOTONIC, &t) != 0)
		pc_unresolved("clock_gettime(CLOCK_MONOTONIC) failed");
	return (long long)t.tv_sec * 1000000000LL + t.tv_nsec;
}

int main(void)
{
	struct timespec res, ts;
	long long c0, c1, w0;
	volatile unsigned long spin = 0;

	pc_begin("XSH.clock_getres.008");
	pc_test();
	pc_check(clock_getres(CLOCK_PROCESS_CPUTIME_ID, &res) == 0,
	    "clock_getres(CLOCK_PROCESS_CPUTIME_ID) failed: %s", pc_errname(errno));
	/* Computing: the clock must move (within 5 s of wall time). */
	c0 = ns_of(CLOCK_PROCESS_CPUTIME_ID, "CLOCK_PROCESS_CPUTIME_ID");
	w0 = wall();
	do {
		int k;
		for (k = 0; k < 100000; k++)
			spin++;
		c1 = ns_of(CLOCK_PROCESS_CPUTIME_ID, "CLOCK_PROCESS_CPUTIME_ID");
	} while (c1 <= c0 && wall() - w0 < 5000000000LL);
	pc_check(c1 > c0, "CLOCK_PROCESS_CPUTIME_ID did not advance while the process computed for 5 s");
	/* Sleeping for 500 ms: the process does not execute. */
	c0 = ns_of(CLOCK_PROCESS_CPUTIME_ID, "CLOCK_PROCESS_CPUTIME_ID");
	ts.tv_sec = 0;
	ts.tv_nsec = 500000000L;
	while (nanosleep(&ts, &ts) == -1 && errno == EINTR)
		;
	c1 = ns_of(CLOCK_PROCESS_CPUTIME_ID, "CLOCK_PROCESS_CPUTIME_ID");
	pc_check(c1 - c0 < 250000000LL, "CLOCK_PROCESS_CPUTIME_ID advanced %lld ms while the process slept 500 ms",
	    (c1 - c0) / 1000000LL);
	pc_pass();
}
