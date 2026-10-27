/*
 * The return value counts the entries with a non-zero revents.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.poll.005
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
	int v, a[2], b[2], c[2], n, e;
	struct pollfd f[3];

	pc_begin("XSH.poll.005");
	guard(30);
	pc_test();
	for (v = 0; v < 2; v++) {
		mkpipe(a);
		mkpipe(b);
		mkpipe(c);
		putbyte(a[1]);
		putbyte(c[1]);
		f[0].fd = a[0]; f[0].events = POLLIN; f[0].revents = 0;
		f[1].fd = b[0]; f[1].events = POLLIN; f[1].revents = POLLIN | POLLOUT;
		f[2].fd = c[0]; f[2].events = POLLIN; f[2].revents = 0;
		errno = 0;
		n = callpoll(v, f, 3, 5000);
		e = errno;
		pc_check(n == 2, "%s returned %d (errno %s), expected 2", fname[v], n, ERRS(n, e));
		pc_check((f[0].revents & POLLIN) != 0, "%s: entry 0 has no POLLIN", fname[v]);
		pc_check(f[1].revents == 0, "%s: the entry that is not ready has revents 0x%x, expected 0", fname[v], (unsigned)f[1].revents);
		pc_check((f[2].revents & POLLIN) != 0, "%s: entry 2 has no POLLIN", fname[v]);
		close(a[0]); close(a[1]); close(b[0]); close(b[1]); close(c[0]); close(c[1]);
	}
	pc_pass();
}
