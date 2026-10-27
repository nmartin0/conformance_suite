/*
 * timespec_get() agrees with time().
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.timespec_get.002
 */
#include "pc.h"
#include <time.h>

int main(void)
{
	struct timespec ts;
	time_t a, b;

	pc_begin("XSH.timespec_get.002");
	pc_test();
	a = time(NULL);
	pc_check(timespec_get(&ts, TIME_UTC) == TIME_UTC, "timespec_get() failed");
	b = time(NULL);
	pc_check(ts.tv_sec >= a - 1 && ts.tv_sec <= b + 1, "tv_sec %ld is not within a second of time() (%ld..%ld)",
	    (long)ts.tv_sec, (long)a, (long)b);
	pc_pass();
}
