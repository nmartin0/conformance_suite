/*
 * tv_nsec of clock readings is in range.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.time.h.006
 */
#include <time.h>
#include "pc.h"

static void check(const char *who, const struct timespec *t)
{
	pc_check(t->tv_nsec >= 0 && t->tv_nsec <= 999999999L, "%s: tv_nsec is %ld", who, (long)t->tv_nsec);
}

int main(void)
{
	pc_begin("XBD.time.h.006");
	pc_test();
	struct timespec t;
	int i;

	for (i = 0; i < 3; i++) {
		pc_check(clock_gettime(CLOCK_REALTIME, &t) == 0, "clock_gettime(CLOCK_REALTIME) failed");
		check("clock_gettime(CLOCK_REALTIME)", &t);
		pc_check(clock_gettime(CLOCK_MONOTONIC, &t) == 0, "clock_gettime(CLOCK_MONOTONIC) failed");
		check("clock_gettime(CLOCK_MONOTONIC)", &t);
		pc_check(timespec_get(&t, TIME_UTC) == TIME_UTC, "timespec_get(TIME_UTC) failed");
		check("timespec_get", &t);
	}
	pc_check(clock_getres(CLOCK_REALTIME, &t) == 0, "clock_getres(CLOCK_REALTIME) failed");
	check("clock_getres(CLOCK_REALTIME)", &t);
	pc_check(clock_getres(CLOCK_MONOTONIC, &t) == 0, "clock_getres(CLOCK_MONOTONIC) failed");
	check("clock_getres(CLOCK_MONOTONIC)", &t);
	pc_pass();
}
