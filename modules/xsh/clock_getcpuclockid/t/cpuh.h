/*
 * cpuh.h - helpers shared by the CPU-time clock tests (copied unchanged into
 * each module that uses them).
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 */
#ifndef CPUH_H
#define CPUH_H

#include "pc.h"
#include <errno.h>
#include <time.h>

/* Reading of clock id in ns; -1 when clock_gettime() fails or the time is invalid. */
static inline long long cpu_ns(clockid_t id)
{
	struct timespec t;

	if (clock_gettime(id, &t) != 0)
		return -1;
	if (t.tv_sec < 0 || t.tv_nsec < 0 || t.tv_nsec >= 1000000000L)
		return -1;
	return (long long)t.tv_sec * 1000000000LL + t.tv_nsec;
}

static inline long long mono_ns(void)
{
	struct timespec t;

	if (clock_gettime(CLOCK_MONOTONIC, &t) != 0)
		return 0;
	return (long long)t.tv_sec * 1000000000LL + t.tv_nsec;
}

/*
 * Burn CPU in the calling thread until clock id has advanced by 5 ms; gives
 * up after 5 s of wall time. Returns the advance in ns (0 if none).
 */
static inline long long cpu_burn(clockid_t id)
{
	volatile unsigned long x = 0;
	long long a = cpu_ns(id), b, w = mono_ns();
	unsigned long i;

	if (a < 0)
		return 0;
	for (;;) {
		for (i = 0; i < 200000; i++)
			x += i;
		b = cpu_ns(id);
		if (b < 0)
			return 0;
		if (b - a >= 5000000LL)
			return b - a;
		if (mono_ns() - w > 5000000000LL)
			return 0;
	}
}

#endif
