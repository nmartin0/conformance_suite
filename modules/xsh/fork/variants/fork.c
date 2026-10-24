/*
 * fork.c - conforming variants for the fork() tests (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; see core/pcvariant.h. Each variant says why the
 * standard allows it.
 */
#define _GNU_SOURCE 1
#include <errno.h>
#include <signal.h>
#include <time.h>
#include "pcvariant.h"

/*
 * "timer_late": every relative per-process timer expires one second later
 * than asked. XSH timer_settime() bounds expiry from below only (a timer
 * never expires before its time; values are rounded up to the clock's
 * resolution) and states no upper bound, so a loaded or coarse system may
 * expire it late. Modelled as a later expiry, so timer_gettime() agrees.
 */
int timer_settime(timer_t t, int flags, const struct itimerspec *value, struct itimerspec *ovalue)
{
	const char *v = variant_of("fork.");
	struct itimerspec late;
	REAL(timer_settime);

	if (IS("timer_late") && value != NULL && !(flags & TIMER_ABSTIME) &&
	    (value->it_value.tv_sec != 0 || value->it_value.tv_nsec != 0)) {
		late = *value;
		late.it_value.tv_sec += 1;
		return real_timer_settime(t, flags, &late, ovalue);
	}
	return real_timer_settime(t, flags, value, ovalue);
}
