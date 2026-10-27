/*
 * times() returns elapsed real time in ticks.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.times.001
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <sys/times.h>

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

static void sleep_ms(long ms)
{
	struct timespec ts;

	ts.tv_sec = ms / 1000;
	ts.tv_nsec = (ms % 1000) * 1000000L;
	while (nanosleep(&ts, &ts) == -1 && errno == EINTR)
		;
}

static long now_ms(void)
{
	struct timespec t;

	clock_gettime(CLOCK_MONOTONIC, &t);
	return t.tv_sec * 1000L + t.tv_nsec / 1000000L;
}

/* One reading of times(); an overflow (allowed) makes the test UNRESOLVED, any other failure is a FAIL. */
static clock_t ticks(struct tms *b)
{
	clock_t t;

	errno = 0;
	t = times(b);
	if (t == (clock_t)-1) {
		if (errno == EOVERFLOW)
			pc_unresolved("times() reported EOVERFLOW (allowed)");
		pc_fail("times() failed: %s", pc_errname(errno));
	}
	return t;
}

static double tick_rate(void)
{
	long hz = sysconf(_SC_CLK_TCK);

	if (hz <= 0)
		pc_unresolved("sysconf(_SC_CLK_TCK) gave %ld", hz);
	return (double)hz;
}

int main(void)
{
	struct tms b;
	clock_t t1, tm, t2;
	double hz, d, secs;
	long r0, r1;

	pc_begin("XSH.times.001");
	hz = tick_rate();
	pc_test();
	t1 = ticks(&b);
	r0 = now_ms();
	sleep_ms(750);
	tm = ticks(&b);
	sleep_ms(750);
	r1 = now_ms();
	t2 = ticks(&b);
	if ((double)t2 < (double)t1)
		pc_unresolved("clock_t wrapped around between the readings");
	d = (double)t2 - (double)t1;
	secs = (double)(r1 - r0) / 1000.0;
	pc_check((double)tm >= (double)t1 && (double)tm <= (double)t2, "a reading in between (%.0f) is outside the two others (%.0f, %.0f)",
	    (double)tm, (double)t1, (double)t2);
	pc_check(d / hz >= secs / 3.0, "%.0f ticks (%.2f s at %.0f/s) passed in %.2f s of real time: too few", d, d / hz, hz, secs);
	pc_check(d / hz <= secs * 4.0, "%.0f ticks (%.2f s at %.0f/s) passed in %.2f s of real time: too many", d, d / hz, hz, secs);
	pc_pass();
}
