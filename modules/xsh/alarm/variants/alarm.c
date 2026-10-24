/*
 * alarm.c - conforming variants of alarm(), for the variant checks (CI
 * only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; see core/pcvariant.h. Each variant says why the
 * standard allows it. alarm() is rebuilt on setitimer(ITIMER_REAL), which
 * drives the same SIGALRM.
 */
#define _GNU_SOURCE 1
#include <errno.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>
#include "pcvariant.h"

/*
 * "ret_trunc" and "ret_ceil": alarm() RETURN VALUE asks for a non-zero
 * number of seconds when the earlier request has time left, without
 * saying how a fraction is rounded. ret_trunc drops the fraction (giving
 * 1 when under a second is left); ret_ceil rounds any fraction up.
 *
 * "early_half": SIGALRM is generated half a second before the time asked
 * for (alarm(1) fires after 0.5 s). alarm()'s RATIONALE records that
 * historical systems fired up to a second early, and whether the text
 * forbids that is open (Q-14, the disputed XSH.alarm.006 and .007). The
 * variant is declared only by the required alarm tests, which must hold
 * under either reading; the return value is then the time left until
 * that earlier generation, rounded to the nearest second (never 0).
 */
unsigned alarm(unsigned seconds)
{
	const char *v = variant_of("alarm.");
	struct itimerval nv, ov;
	long long us;
	unsigned r;
	int e = errno;
	REAL(alarm);

	if (!IS("ret_trunc") && !IS("ret_ceil") && !IS("early_half"))
		return real_alarm(seconds);
	nv.it_interval.tv_sec = 0;
	nv.it_interval.tv_usec = 0;
	us = (long long)seconds * 1000000LL;
	if (seconds > 0 && IS("early_half"))
		us -= 500000LL;
	nv.it_value.tv_sec = (time_t)(us / 1000000LL);
	nv.it_value.tv_usec = (suseconds_t)(us % 1000000LL);
	if (setitimer(ITIMER_REAL, &nv, &ov) != 0)
		return real_alarm(seconds);
	errno = e;
	r = (unsigned)ov.it_value.tv_sec;
	if (IS("ret_ceil") && ov.it_value.tv_usec > 0)
		r++;
	if (IS("early_half") && ov.it_value.tv_usec >= 500000)
		r++;
	if (r == 0 && ov.it_value.tv_usec > 0)
		r = 1;
	return r;
}
