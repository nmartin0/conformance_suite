/*
 * tms_stime counts system CPU time.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.times.003
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

/* Writes bytes to fd, ignoring the result (used where nothing can be done about it). */
static void put(int fd, const void *p, size_t n)
{
	ssize_t w = write(fd, p, n);

	(void)w;
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

/* Spends about ms of CPU time (by clock()) in many small system calls. */
static void burn_system(long ms)
{
	clock_t end = clock() + (clock_t)((double)ms / 1000.0 * (double)CLOCKS_PER_SEC);
	long real0 = now_ms();
	int fd = open("/dev/null", O_WRONLY);
	int i;

	if (fd < 0)
		pc_unresolved("cannot open /dev/null: %s", pc_errname(errno));
	while (clock() < end && now_ms() - real0 < 30000)
		for (i = 0; i < 2000; i++)
			put(fd, "x", 1);
	(void)close(fd);
}

int main(void)
{
	struct tms a, b;
	double d;

	pc_begin("XSH.times.003");
	pc_test();
	(void)ticks(&a);
	burn_system(500);
	(void)ticks(&b);
	d = (double)b.tms_stime - (double)a.tms_stime;
	pc_check(d >= 1, "tms_stime grew by %.0f ticks over about 0.5 s of CPU time spent mostly in system calls", d);
	pc_pass();
}
