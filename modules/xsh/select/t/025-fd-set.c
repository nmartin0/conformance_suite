/*
 * FD_SET() adds a descriptor, once.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.select.025
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
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/un.h>
#include <sys/wait.h>
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

/* v 0: select(), v 1: pselect() with a null mask; ms < 0: no timeout. */
static const char *const fname[2] = { "select", "pselect" };

static int callsel(int v, int nfds, fd_set *r, fd_set *w, fd_set *e, long long ms)
{
	if (v == 0) {
		struct timeval tv;

		tv.tv_sec = (time_t)(ms / 1000);
		tv.tv_usec = (suseconds_t)((ms % 1000) * 1000);
		return select(nfds, r, w, e, ms < 0 ? NULL : &tv);
	} else {
		struct timespec ts;

		ts.tv_sec = (time_t)(ms / 1000);
		ts.tv_nsec = (long)((ms % 1000) * 1000000L);
		return pselect(nfds, r, w, e, ms < 0 ? NULL : &ts, NULL);
	}
}

static int maxi(int a, int b)
{
	return a > b ? a : b;
}

/* Number of members of s in 0..FD_SETSIZE-1. */
static int members(const fd_set *s)
{
	int i, n = 0;

	for (i = 0; i < FD_SETSIZE; i++)
		if (FD_ISSET(i, s))
			n++;
	return n;
}

static int same_set(const fd_set *a, const fd_set *b)
{
	int i;

	for (i = 0; i < FD_SETSIZE; i++)
		if ((FD_ISSET(i, a) != 0) != (FD_ISSET(i, b) != 0))
			return 0;
	return 1;
}

int main(void)
{
	fd_set s;
	int i, want;

	pc_begin("XSH.select.025");
	pc_test();
	FD_ZERO(&s);
	FD_SET(0, &s);
	FD_SET(7, &s);
	FD_SET(FD_SETSIZE - 1, &s);
	for (i = 0; i < FD_SETSIZE; i++) {
		want = (i == 0 || i == 7 || i == FD_SETSIZE - 1);
		pc_check((FD_ISSET(i, &s) != 0) == want, "descriptor %d: FD_ISSET() says %s, expected %s", i,
		    FD_ISSET(i, &s) ? "member" : "not a member", want ? "member" : "not a member");
	}
	FD_SET(7, &s);
	FD_CLR(7, &s);
	pc_check(!FD_ISSET(7, &s), "FD_SET() twice then FD_CLR() once left the descriptor in the set");
	pc_check(FD_ISSET(0, &s) && FD_ISSET(FD_SETSIZE - 1, &s), "other members were lost");
	pc_pass();
}
