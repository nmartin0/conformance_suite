/*
 * clock_getres.c - conforming variants of clock_getres(), clock_gettime()
 * and clock_settime(), for the variant checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; see core/pcvariant.h. Each variant says why the
 * standard allows it.
 */
#define _GNU_SOURCE 1
#include <errno.h>
#include <time.h>
#include "pcvariant.h"

#define EXTRA_ID ((clockid_t)0x7ffffff0L)
#define STEP_NS 10000000L

/*
 * "coarse": CLOCK_REALTIME and CLOCK_MONOTONIC tick in 10 ms steps.
 * clock_getres() DESCRIPTION makes resolutions implementation-defined; the
 * values read are then whole multiples of the resolution (truncated, as
 * a clock that steps would show them), and clock_getres() reports 10 ms.
 *
 * "extra_clock": the implementation has an additional clock with ID
 * 0x7ffffff0 (DESCRIPTION: an implementation may support additional
 * clocks, whose values have an unspecified meaning); here it reads the
 * monotonic clock and, like it, cannot be set.
 *
 * "settime_eperm": no process may set any clock. Which privileges set a
 * clock is implementation-defined (DESCRIPTION) and EPERM is a may-fail
 * error of clock_settime(); where another error applies as well, XSH 2.3
 * lets either be reported.
 */
static int coarse_id(clockid_t id)
{
	return id == CLOCK_REALTIME || id == CLOCK_MONOTONIC;
}

int clock_getres(clockid_t id, struct timespec *res)
{
	const char *v = variant_of("clock_getres.");
	REAL(clock_getres);

	if (IS("coarse") && coarse_id(id)) {
		if (res != NULL) {
			res->tv_sec = 0;
			res->tv_nsec = STEP_NS;
		}
		return 0;
	}
	if (IS("extra_clock") && id == EXTRA_ID)
		return real_clock_getres(CLOCK_MONOTONIC, res);
	return real_clock_getres(id, res);
}

int clock_gettime(clockid_t id, struct timespec *tp)
{
	const char *v = variant_of("clock_getres.");
	int r;
	REAL(clock_gettime);

	if (IS("extra_clock") && id == EXTRA_ID)
		id = CLOCK_MONOTONIC;
	r = real_clock_gettime(id, tp);
	if (r == 0 && IS("coarse") && coarse_id(id))
		tp->tv_nsec -= tp->tv_nsec % STEP_NS;
	return r;
}

int clock_settime(clockid_t id, const struct timespec *tp)
{
	const char *v = variant_of("clock_getres.");
	REAL(clock_settime);

	if (IS("settime_eperm")) {
		errno = EPERM;
		return -1;
	}
	if (IS("extra_clock") && id == EXTRA_ID) {
		errno = EINVAL;
		return -1;
	}
	return real_clock_settime(id, tp);
}
