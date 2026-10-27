/*
 * Child times add up through the generations.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.times.007
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

/* A call that blocks must fail the test, not hang it. */
static void on_guard(int sig)
{
	(void)sig;
	pc_fail("blocked: the call under test did not finish in time");
}

static void guard(unsigned secs)
{
	struct sigaction sa;

	memset(&sa, 0, sizeof sa);
	sa.sa_handler = on_guard;
	sigemptyset(&sa.sa_mask);
	pc_check(sigaction(SIGALRM, &sa, NULL) == 0, "sigaction failed");
	alarm(secs);
}

static long now_ms(void)
{
	struct timespec t;

	clock_gettime(CLOCK_MONOTONIC, &t);
	return t.tv_sec * 1000L + t.tv_nsec / 1000000L;
}

/* Reads from fd until end-of-file or n bytes; returns the count. */
static size_t slurp(int fd, void *buf, size_t n)
{
	unsigned char *b = buf;
	size_t got = 0;
	ssize_t k;

	while (got < n) {
		k = read(fd, b + got, n - got);
		if (k > 0)
			got += (size_t)k;
		else if (k == 0)
			break;
		else if (errno != EINTR)
			pc_unresolved("read failed: %s", pc_errname(errno));
	}
	return got;
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
/* Returns 0 if the real-time cap ended it before that much CPU time was used. */
static int burn_user(long ms)
{
	volatile unsigned long x = 1;
	clock_t end = clock() + (clock_t)((double)ms / 1000.0 * (double)CLOCKS_PER_SEC);
	long real0 = now_ms();
	unsigned long i;

	while (clock() < end && now_ms() - real0 < 30000)
		for (i = 0; i < 1000000; i++)
			x = x * 1664525UL + 1013904223UL;
	return clock() >= end;
}

/*
 * CPU time to spend, in ms: at least 0.5 s, and at least 20 clock ticks, so
 * that a coarse tick (sysconf(_SC_CLK_TCK) small) still shows growth of one
 * tick. A tick rate that cannot be read makes the test UNRESOLVED.
 */
static long burn_ms(void)
{
	long hz = sysconf(_SC_CLK_TCK), ms;

	if (hz <= 0)
		pc_unresolved("sysconf(_SC_CLK_TCK) gave %ld", hz);
	ms = (20L * 1000L + hz - 1) / hz;
	if (ms < 500)
		ms = 500;
	if (ms > 20000)
		ms = 20000;
	return ms;
}

int main(void)
{
	struct tms snap, mine;
	double d;
	long ms;
	pid_t c;
	int p[2], st;

	pc_begin("XSH.times.007");
	guard(90);
	pc_check(pipe(p) == 0, "pipe failed");
	pc_test();
	ms = burn_ms();
	c = fork();
	if (c == -1)
		pc_unresolved("fork failed: %s", pc_errname(errno));
	if (c == 0) {
		struct tms s;
		pid_t g;
		int gst;

		(void)close(p[0]);
		g = fork();
		if (g == -1)
			_exit(91);
		if (g == 0) {
			_exit(burn_user(ms) ? 0 : 95);
		}
		if (waitpid(g, &gst, 0) != g)
			_exit(92);
		if (!WIFEXITED(gst) || WEXITSTATUS(gst) != 0)
			_exit(95);
		if (times(&s) == (clock_t)-1)
			_exit(93);
		if (write(p[1], &s, sizeof s) != (ssize_t)sizeof s)
			_exit(94);
		_exit(0);
	}
	(void)close(p[1]);
	if (slurp(p[0], &snap, sizeof snap) != sizeof snap)
		pc_unresolved("the child did not report its times");
	if (waitpid(c, &st, 0) != c)
		pc_unresolved("waitpid failed: %s", pc_errname(errno));
	if (!WIFEXITED(st) || WEXITSTATUS(st) != 0)
		pc_unresolved("the child failed (status %#x)", st);
	d = (double)snap.tms_cutime + (double)snap.tms_cstime;
	pc_check(d >= 1, "a child that waited for its own child (at least 0.5 s of CPU) had tms_cutime + tms_cstime of %.0f ticks", d);
	(void)ticks(&mine);
	pc_check((double)mine.tms_cutime >= (double)snap.tms_utime + (double)snap.tms_cutime,
	    "tms_cutime %.0f is below the child's tms_utime + tms_cutime (%.0f + %.0f) read before it ended",
	    (double)mine.tms_cutime, (double)snap.tms_utime, (double)snap.tms_cutime);
	pc_check((double)mine.tms_cstime >= (double)snap.tms_stime + (double)snap.tms_cstime,
	    "tms_cstime %.0f is below the child's tms_stime + tms_cstime (%.0f + %.0f) read before it ended",
	    (double)mine.tms_cstime, (double)snap.tms_stime, (double)snap.tms_cstime);
	pc_pass();
}
