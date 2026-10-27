/*
 * An invalid who fails with EINVAL.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getrusage.005
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <signal.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/un.h>
#include <sys/wait.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

/* After pc_test(): a step that is not under test gives UNRESOLVED, not FAIL. */
#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)
#define MS 1000000LL

/* A call that blocks must not hang the test. Also makes sure SIGCHLD has its default action, so that children can be waited for. */
static void on_guard(int sig)
{
	(void)sig;
	pc_unresolved("a call did not finish within the guard time");
}

static void guard(unsigned secs)
{
	struct sigaction sa;

	memset(&sa, 0, sizeof sa);
	sa.sa_handler = on_guard;
	sigemptyset(&sa.sa_mask);
	NEED(sigaction(SIGALRM, &sa, NULL) == 0, "sigaction failed");
	(void)signal(SIGCHLD, SIG_DFL);
	alarm(secs);
}

static long long mono(void)
{
	struct timespec t;

	NEED(clock_gettime(CLOCK_MONOTONIC, &t) == 0, "clock_gettime failed: %s", pc_errname(errno));
	return (long long)t.tv_sec * 1000000000LL + t.tv_nsec;
}

static long long mono_res(void)
{
	struct timespec r;

	NEED(clock_getres(CLOCK_MONOTONIC, &r) == 0, "clock_getres failed: %s", pc_errname(errno));
	return (long long)r.tv_sec * 1000000000LL + r.tv_nsec;
}

static void mkpipe(int p[2])
{
	NEED(pipe(p) == 0, "pipe failed: %s", pc_errname(errno));
}

static void putbyte(int fd)
{
	NEED(write(fd, "x", 1) == 1, "write failed: %s", pc_errname(errno));
}

static void nap_ms(long ms)
{
	struct timespec ts;

	ts.tv_sec = ms / 1000;
	ts.tv_nsec = (ms % 1000) * 1000000L;
	while (nanosleep(&ts, &ts) == -1 && errno == EINTR)
		;
}

/* A busy loop until it has used cpu_ms of processor time (by clock()) or wall_cap_ms have passed: 0 reached, -1 not. No pc_* calls: usable in a child. */
static int burn_cpu(long cpu_ms, long wall_cap_ms)
{
	struct timespec a, b;
	clock_t c0 = clock();
	volatile unsigned long x;

	(void)clock_gettime(CLOCK_MONOTONIC, &a);
	for (;;) {
		for (x = 0; x < 200000UL; x++)
			;
		(void)clock_gettime(CLOCK_MONOTONIC, &b);
		if (c0 != (clock_t)-1 && clock() != (clock_t)-1) {
			if ((long)((clock() - c0) / (CLOCKS_PER_SEC / 1000)) >= cpu_ms)
				return 0;
		} else if ((b.tv_sec - a.tv_sec) * 1000 + (b.tv_nsec - a.tv_nsec) / 1000000 >= cpu_ms) {
			return 0;
		}
		if ((b.tv_sec - a.tv_sec) * 1000 + (b.tv_nsec - a.tv_nsec) / 1000000 >= wall_cap_ms)
			return -1;
	}
}

static unsigned long long total_us(const struct rusage *u)
{
	return ((unsigned long long)u->ru_utime.tv_sec + (unsigned long long)u->ru_stime.tv_sec) * 1000000ULL
	    + (unsigned long long)u->ru_utime.tv_usec + (unsigned long long)u->ru_stime.tv_usec;
}

static unsigned long long usage(int who)
{
	struct rusage u;

	memset(&u, 0, sizeof u);
	if (getrusage(who, &u) != 0)
		pc_fail("getrusage(%s) failed: %s", who == RUSAGE_SELF ? "RUSAGE_SELF" : "RUSAGE_CHILDREN", pc_errname(errno));
	return total_us(&u);
}

static void reap(pid_t c, int *st)
{
	while (waitpid(c, st, 0) == -1)
		NEED(errno == EINTR, "waitpid failed: %s", pc_errname(errno));
}

int main(void)
{
	static const int cand[] = { -1, INT_MAX, INT_MIN };
	struct rusage u;
	size_t i;
	int r, refused = 0;

	pc_begin("XSH.getrusage.005");
	pc_test();
	for (i = 0; i < sizeof cand / sizeof cand[0]; i++) {
		errno = 0;
		r = getrusage(cand[i], &u);
		if (r == 0)
			continue;
		refused++;
		pc_check(r == -1 && errno == EINVAL, "getrusage(%d) returned %d with errno %s, expected -1 with EINVAL", cand[i], r, pc_errname(errno));
	}
	pc_check(refused > 0, "getrusage() accepted -1, INT_MAX and INT_MIN as who");
	pc_pass();
}
