/*
 * nanosleep.c - conforming variants of nanosleep(), for the variant checks
 * (CI only).
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

#define LATE_NS 300000000L
#define STEP_NS 10000000LL

/*
 * "late": a sleep that ran its full time returns 300 ms after it, as on a
 * busy system. nanosleep() DESCRIPTION allows a longer suspension for the
 * scheduling of other activity, and gives no upper bound; the call has
 * still returned because the time elapsed, so it returns 0. A signal
 * during the extra time is handled but does not change that.
 *
 * "rem_coarse": the remaining time stored in *rmtp after an interruption
 * is rounded up to a whole 10 ms (never beyond the time asked for), as by
 * a sleep that counts in timer ticks. RETURN VALUE asks for the requested
 * time minus the time slept; a system whose sleeps are rounded up to its
 * sleep resolution (DESCRIPTION) only knows the time slept to that
 * resolution.
 */
int nanosleep(const struct timespec *rq, struct timespec *rm)
{
	const char *v = variant_of("nanosleep.");
	struct timespec req, extra;
	int r, e;
	REAL(nanosleep);

	if (v == NULL || rq == NULL)
		return real_nanosleep(rq, rm);
	req = *rq;
	r = real_nanosleep(rq, rm);
	e = errno;
	if (r == 0 && IS("late")) {
		extra.tv_sec = 0;
		extra.tv_nsec = LATE_NS;
		while (real_nanosleep(&extra, &extra) == -1 && errno == EINTR)
			;
	}
	if (r == -1 && e == EINTR && IS("rem_coarse") && rm != NULL) {
		long long left = (long long)rm->tv_sec * 1000000000LL + rm->tv_nsec;
		long long cap = (long long)req.tv_sec * 1000000000LL + req.tv_nsec;

		left = (left + STEP_NS - 1) / STEP_NS * STEP_NS;
		if (left > cap)
			left = cap;
		rm->tv_sec = (time_t)(left / 1000000000LL);
		rm->tv_nsec = (long)(left % 1000000000LL);
	}
	errno = e;
	return r;
}
