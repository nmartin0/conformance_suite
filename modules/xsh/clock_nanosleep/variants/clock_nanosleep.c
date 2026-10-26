/*
 * clock_nanosleep.c - conforming variants of clock_nanosleep(), for the
 * variant checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; see core/pcvariant.h. Each variant says why the
 * standard allows it.
 */
#define _GNU_SOURCE 1
#include <errno.h>
#include <pthread.h>
#include <time.h>
#include "pcvariant.h"

#define LATE_NS 300000000L
#define STEP_NS 10000000LL

/*
 * "late": a sleep that ran its full time, relative or absolute, returns
 * 300 ms after it. DESCRIPTION allows a longer suspension for scheduling
 * and gives no upper bound; the call returns 0, since the time elapsed.
 *
 * "rem_coarse": after an interrupted relative sleep, the time left is
 * rounded up to a whole 10 ms (never beyond the time asked for), as by a
 * system whose sleeps count in ticks (DESCRIPTION: sleeps are rounded up
 * to the sleep resolution).
 *
 * "enotsup_cpu": for the calling thread's CPU-time clock the call returns
 * ENOTSUP rather than EINVAL. ERRORS names that clock under EINVAL and
 * gives a CPU-time clock as the example for ENOTSUP; with both conditions
 * holding, XSH 2.3 allows either.
 */
int clock_nanosleep(clockid_t id, int flags, const struct timespec *rq, struct timespec *rm)
{
	const char *v = variant_of("clock_nanosleep.");
	struct timespec req, extra;
	clockid_t self;
	int r;
	REAL(clock_nanosleep);

	if (v == NULL || rq == NULL)
		return real_clock_nanosleep(id, flags, rq, rm);
	if (IS("enotsup_cpu") && (id == CLOCK_THREAD_CPUTIME_ID ||
	    (pthread_getcpuclockid(pthread_self(), &self) == 0 && id == self)))
		return ENOTSUP;
	req = *rq;
	r = real_clock_nanosleep(id, flags, rq, rm);
	if (r == 0 && IS("late")) {
		extra.tv_sec = 0;
		extra.tv_nsec = LATE_NS;
		while (real_clock_nanosleep(CLOCK_MONOTONIC, 0, &extra, &extra) == EINTR)
			;
	}
	if (r == EINTR && IS("rem_coarse") && rm != NULL && !(flags & TIMER_ABSTIME)) {
		long long left = (long long)rm->tv_sec * 1000000000LL + rm->tv_nsec;
		long long cap = (long long)req.tv_sec * 1000000000LL + req.tv_nsec;

		left = (left + STEP_NS - 1) / STEP_NS * STEP_NS;
		if (left > cap)
			left = cap;
		rm->tv_sec = (time_t)(left / 1000000000LL);
		rm->tv_nsec = (long)(left % 1000000000LL);
	}
	return r;
}
