/*
 * CLOCK_MONOTONIC never goes back, and it moves.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.clock_getres.005
 * MUTANT    clock_getres.monotonic_jitter
 * MUTANT    clock_getres.monotonic_frozen
 * VARIANT   clock_getres.coarse
 */
#include "pc.h"
#include <errno.h>
#include <poll.h>
#include <time.h>

static int before(const struct timespec *a, const struct timespec *b)
{
	return a->tv_sec < b->tv_sec || (a->tv_sec == b->tv_sec && a->tv_nsec < b->tv_nsec);
}

int main(void)
{
	struct timespec first, prev, cur;
	long i;
	int moved = 0;

	pc_begin("XSH.clock_getres.005");
	pc_test();
	pc_check(clock_gettime(CLOCK_MONOTONIC, &first) == 0, "clock_gettime(CLOCK_MONOTONIC) failed: %s", pc_errname(errno));
	prev = first;
	for (i = 0; i < 100000; i++) {
		pc_check(clock_gettime(CLOCK_MONOTONIC, &cur) == 0, "clock_gettime(CLOCK_MONOTONIC) failed: %s", pc_errname(errno));
		pc_check(!before(&cur, &prev), "CLOCK_MONOTONIC went back from %lld.%09ld to %lld.%09ld",
		    (long long)prev.tv_sec, (long)prev.tv_nsec, (long long)cur.tv_sec, (long)cur.tv_nsec);
		if (before(&first, &cur))
			moved = 1;
		prev = cur;
	}
	/* Then wait (poll() with a timeout, 10 ms at a time) up to 2 s for it to move. */
	for (i = 0; !moved && i < 200; i++) {
		(void)poll(NULL, 0, 10);
		pc_check(clock_gettime(CLOCK_MONOTONIC, &cur) == 0, "clock_gettime(CLOCK_MONOTONIC) failed: %s", pc_errname(errno));
		pc_check(!before(&cur, &prev), "CLOCK_MONOTONIC went back");
		if (before(&first, &cur))
			moved = 1;
		prev = cur;
	}
	pc_check(moved, "CLOCK_MONOTONIC did not change in about 2 s");
	pc_pass();
}
