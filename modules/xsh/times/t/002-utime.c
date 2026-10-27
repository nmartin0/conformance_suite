/*
 * tms_utime counts user CPU time.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.times.002
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

/* Spends about ms of CPU time (by clock()) on arithmetic: nearly all of it in user mode. */
static void burn_user(long ms)
{
	volatile unsigned long x = 1;
	clock_t end = clock() + (clock_t)((double)ms / 1000.0 * (double)CLOCKS_PER_SEC);
	long real0 = now_ms();
	unsigned long i;

	while (clock() < end && now_ms() - real0 < 30000)
		for (i = 0; i < 1000000; i++)
			x = x * 1664525UL + 1013904223UL;
}

int main(void)
{
	struct tms a, b;
	double d;

	pc_begin("XSH.times.002");
	pc_test();
	(void)ticks(&a);
	burn_user(500);
	(void)ticks(&b);
	d = (double)b.tms_utime - (double)a.tms_utime;
	pc_check(d >= 1, "tms_utime grew by %.0f ticks over about 0.5 s of CPU time in user mode", d);
	pc_pass();
}
