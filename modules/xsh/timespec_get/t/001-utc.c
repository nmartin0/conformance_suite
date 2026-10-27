/*
 * timespec_get() with TIME_UTC.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.timespec_get.001
 */
#include "pc.h"
#include <time.h>

static double secs(struct timespec t)
{
	return (double)t.tv_sec + (double)t.tv_nsec / 1e9;
}

int main(void)
{
	struct timespec before, ts, after;
	int r;

	pc_begin("XSH.timespec_get.001");
	pc_check(clock_gettime(CLOCK_REALTIME, &before) == 0, "clock_gettime failed");
	pc_test();
	ts.tv_sec = 0;
	ts.tv_nsec = -1;
	r = timespec_get(&ts, TIME_UTC);
	pc_check(clock_gettime(CLOCK_REALTIME, &after) == 0, "clock_gettime failed");
	pc_check(r == TIME_UTC, "timespec_get() returned %d, expected TIME_UTC (%d)", r, TIME_UTC);
	pc_check(ts.tv_nsec >= 0 && ts.tv_nsec <= 999999999L, "tv_nsec is %ld", (long)ts.tv_nsec);
	pc_check(secs(ts) >= secs(before) - 1.0, "the time %.3f is earlier than the clock read before (%.3f)", secs(ts), secs(before));
	pc_check(secs(ts) <= secs(after) + 1.0, "the time %.3f is later than the clock read after (%.3f)", secs(ts), secs(after));
	pc_pass();
}
