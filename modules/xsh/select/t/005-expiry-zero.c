/*
 * When the timeout expires the sets are all clear and 0 is returned.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.select.005
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

/* Fills the pipe through its write end until one more byte would block. */
static void fill_pipe(int w)
{
	char buf[4096];
	long total = 0;
	int fl = fcntl(w, F_GETFL);

	NEED(fl != -1, "fcntl(F_GETFL) failed");
	NEED(fcntl(w, F_SETFL, fl | O_NONBLOCK) != -1, "fcntl(F_SETFL) failed");
	memset(buf, 'f', sizeof buf);
	for (;;) {
		ssize_t k = write(w, buf, sizeof buf);

		if (k < 0) {
			NEED(errno == EAGAIN || errno == EWOULDBLOCK, "write to a pipe failed: %s", pc_errname(errno));
			break;
		}
		total += k;
		NEED(total < (64L << 20), "pipe does not fill up");
	}
	for (;;) {
		ssize_t k = write(w, "f", 1);

		if (k < 0) {
			NEED(errno == EAGAIN || errno == EWOULDBLOCK, "write to a pipe failed: %s", pc_errname(errno));
			break;
		}
		total += k;
		NEED(total < (64L << 20), "pipe does not fill up");
	}
	NEED(fcntl(w, F_SETFL, fl) != -1, "fcntl(F_SETFL) failed");
}

int main(void)
{
	int v, a[2], f[2], n, e;
	fd_set r, w;

	pc_begin("XSH.select.005");
	guard(60);
	pc_test();
	for (v = 0; v < 2; v++) {
		mkpipe(a);
		mkpipe(f);
		fill_pipe(f[1]);
		FD_ZERO(&r);
		FD_ZERO(&w);
		FD_SET(a[0], &r);
		FD_SET(f[1], &w);
		errno = 0;
		n = callsel(v, maxi(a[0], f[1]) + 1, &r, &w, NULL, 20);
		e = errno;
		pc_check(n == 0, "%s returned %d (errno %s), expected 0", fname[v], n, n == -1 ? pc_errname(e) : "-");
		pc_check(members(&r) == 0, "%s: readfds still has %d member(s) after the timeout", fname[v], members(&r));
		pc_check(members(&w) == 0, "%s: writefds still has %d member(s) after the timeout", fname[v], members(&w));
		close(a[0]); close(a[1]); close(f[0]); close(f[1]);
	}
	pc_pass();
}
