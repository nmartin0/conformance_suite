/*
 * A listening socket polls ready once a connection is available.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.poll.029
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
	struct pollfd f;

	pc_begin("XSH.poll.029");
	guard(30);
	pc_test();
	for (v = 0; v < 2; v++) {
		s = listen_socket("lsock");
		f.fd = s; f.events = POLLIN; f.revents = 0;
		errno = 0;
		n = callpoll(v, &f, 1, 0);
		e = errno;
		pc_check(n == 0 && f.revents == 0, "%s with no connection pending returned %d (errno %s), revents 0x%x, expected 0 and 0", fname[v], n, ERRS(n, e), (unsigned)f.revents);
		c = client_socket("lsock");
		f.revents = 0;
		errno = 0;
		n = callpoll(v, &f, 1, 5000);
		e = errno;
		pc_check(n == 1, "%s with a connection pending returned %d (errno %s), expected 1", fname[v], n, ERRS(n, e));
		pc_check((f.revents & POLLIN) != 0, "%s: POLLIN is not set (0x%x)", fname[v], (unsigned)f.revents);
		k = accept(s, NULL, NULL);
		pc_check(k != -1, "accept() after %s reported the connection failed: %s", fname[v], pc_errname(errno));
		close(k); close(c); close(s);
		(void)unlink("lsock");
	}
	pc_pass();
}
