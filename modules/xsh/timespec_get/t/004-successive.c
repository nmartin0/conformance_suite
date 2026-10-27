/*
 * Successive timespec_get() calls.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.timespec_get.004
 */
#include "pc.h"
#include <time.h>

static double secs(struct timespec t)
{
	return (double)t.tv_sec + (double)t.tv_nsec / 1e9;
}

int main(void)
{
	struct timespec a, b;

	pc_begin("XSH.timespec_get.004");
	pc_test();
	pc_check(TIME_UTC != 0, "TIME_UTC is zero");
	pc_check(timespec_get(&a, TIME_UTC) == TIME_UTC, "first timespec_get() failed");
	pc_check(timespec_get(&b, TIME_UTC) == TIME_UTC, "second timespec_get() failed");
	pc_check(secs(b) >= secs(a) - 1.0, "the second time %.3f is earlier than the first %.3f", secs(b), secs(a));
	pc_check(secs(b) - secs(a) <= 1.0, "the calls differ by %.3f seconds", secs(b) - secs(a));
	pc_pass();
}
