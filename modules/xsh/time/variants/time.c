/*
 * time.c - conforming variants of time(), for the variant checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; see core/pcvariant.h. Each variant says why the
 * standard allows it.
 */
#define _GNU_SOURCE 1
#include <time.h>
#include "pcvariant.h"

/*
 * "lags": time() reads a clock that is updated once per tick, here 20 ms
 * behind CLOCK_REALTIME. time() DESCRIPTION asks for the time in seconds
 * since the Epoch and says nothing of how fresh the reading is beyond
 * that; a second boundary may thus show up in time() up to a tick later.
 */
time_t time(time_t *tloc)
{
	const char *v = variant_of("time.");
	struct timespec ts;
	time_t r;
	REAL(time);

	if (!IS("lags") || clock_gettime(CLOCK_REALTIME, &ts) != 0)
		return real_time(tloc);
	r = ts.tv_sec;
	if (ts.tv_nsec < 20000000L)
		r--;
	if (tloc != NULL)
		*tloc = r;
	return r;
}
