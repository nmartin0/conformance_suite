/*
 * A listening socket is ready for reading when a connection is pending.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.select.021
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

/* A listening AF_UNIX stream socket bound to a name in the current directory. */
static int listen_socket(const char *name)
{
	struct sockaddr_un a;
	int s = socket(AF_UNIX, SOCK_STREAM, 0);

	NEED(s != -1, "socket failed: %s", pc_errname(errno));
	memset(&a, 0, sizeof a);
	a.sun_family = AF_UNIX;
	NEED(strlen(name) < sizeof a.sun_path, "name too long");
	strcpy(a.sun_path, name);
	(void)unlink(name);
	NEED(bind(s, (struct sockaddr *)&a, sizeof a) == 0, "bind failed: %s", pc_errname(errno));
	NEED(listen(s, 4) == 0, "listen failed: %s", pc_errname(errno));
	return s;
}

/* Connects a new client socket to the listening socket named name. */
static int client_socket(const char *name)
{
	struct sockaddr_un a;
	int c = socket(AF_UNIX, SOCK_STREAM, 0);

	NEED(c != -1, "socket failed: %s", pc_errname(errno));
	memset(&a, 0, sizeof a);
	a.sun_family = AF_UNIX;
	strcpy(a.sun_path, name);
	NEED(connect(c, (struct sockaddr *)&a, sizeof a) == 0, "connect failed: %s", pc_errname(errno));
	return c;
}

int main(void)
{
	int v, s, c, k, n, e;
	fd_set r;

	pc_begin("XSH.select.021");
	guard(30);
	pc_test();
	for (v = 0; v < 2; v++) {
		s = listen_socket("lsock");
		FD_ZERO(&r);
		FD_SET(s, &r);
		errno = 0;
		n = callsel(v, s + 1, &r, NULL, NULL, 0);
		e = errno;
		pc_check(n == 0, "%s with no connection pending returned %d (errno %s), expected 0", fname[v], n, n == -1 ? pc_errname(e) : "-");
		c = client_socket("lsock");
		FD_ZERO(&r);
		FD_SET(s, &r);
		errno = 0;
		n = callsel(v, s + 1, &r, NULL, NULL, 5000);
		e = errno;
		pc_check(n == 1, "%s with a connection pending returned %d (errno %s), expected 1", fname[v], n, n == -1 ? pc_errname(e) : "-");
		pc_check(FD_ISSET(s, &r) != 0, "%s: the listening socket's bit is not set", fname[v]);
		k = accept(s, NULL, NULL);
		pc_check(k != -1, "accept() after %s reported the connection failed: %s", fname[v], pc_errname(errno));
		close(k); close(c); close(s);
		(void)unlink("lsock");
	}
	pc_pass();
}
