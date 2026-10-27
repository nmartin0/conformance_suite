/*
 * nfds above {OPEN_MAX} fails with EINVAL.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.poll.025
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <poll.h>
#include <signal.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

/* v 0: poll(), v 1: ppoll() with a null mask; ms < 0: no timeout. */
static const char *const fname[2] = { "poll", "ppoll" };

static int callpoll(int v, struct pollfd *f, nfds_t n, long long ms)
{
	if (v == 0) {
		return poll(f, n, (int)ms);
	} else {
		struct timespec ts;

		if (ms < 0)
			return ppoll(f, n, NULL, NULL);
		ts.tv_sec = (time_t)(ms / 1000);
		ts.tv_nsec = (long)((ms % 1000) * 1000000L);
		return ppoll(f, n, &ts, NULL);
	}
}

#define ERRS(n, e) ((n) == -1 ? pc_errname(e) : "-")

int main(void)
{
	int v, n, e;
	long m;
	struct pollfd *arr;
	nfds_t i, cnt;

	pc_begin("XSH.poll.025");
	guard(60);
	errno = 0;
	m = sysconf(_SC_OPEN_MAX);
	if (m < 0)
		pc_notinuse("{OPEN_MAX} is indeterminate (sysconf(_SC_OPEN_MAX) gave -1)");
	if (m > (4L << 20))
		pc_notinuse("{OPEN_MAX} is %ld, too large for the test to build the array", m);
	cnt = (nfds_t)m + 1;
	arr = calloc(cnt, sizeof *arr);
	pc_check(arr != NULL, "calloc failed");
	for (i = 0; i < cnt; i++) {
		arr[i].fd = -1;
		arr[i].events = POLLIN;
		arr[i].revents = 0;
	}
	pc_test();
	for (v = 0; v < 2; v++) {
		errno = 0;
		n = callpoll(v, arr, cnt, 0);
		e = errno;
		pc_check(n == -1, "%s with nfds {OPEN_MAX}+1 returned %d, expected -1", fname[v], n);
		pc_check(e == EINVAL, "%s: errno is %s, expected EINVAL", fname[v], pc_errname(e));
	}
	pc_pass();
}
