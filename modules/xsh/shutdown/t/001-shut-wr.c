/*
 * After SHUT_WR the peer reads the queued data and end-of-file; the shut end still receives and stays open.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.shutdown.001
 */
#include "pc.h"
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <netinet/in.h>
#include <poll.h>
#include <signal.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/un.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

/* After pc_test(): a step that is not under test gives UNRESOLVED, not FAIL. */
#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)
#define SAME(a, b) (memcmp(&(a), &(b), sizeof(a)) == 0)

const char *en(int r, int e)
{
	return r == -1 ? pc_errname(e) : "success";
}

/* Is e one of up to four errors (0 = unused)? */
int is_err(int e, int a, int b, int c, int d)
{
	return e == a || (b && e == b) || (c && e == c) || (d && e == d);
}

#ifndef ESOCKTNOSUPPORT
#define ESOCKTNOSUPPORT EPROTONOSUPPORT
#endif

/* A call that blocks must not hang the test. SIGPIPE is ignored, SIGCHLD default. */
void on_guard(int sig)
{
	(void)sig;
	pc_unresolved("a call did not finish within the guard time");
}

void guard(unsigned secs)
{
	struct sigaction sa;

	memset(&sa, 0, sizeof sa);
	sa.sa_handler = on_guard;
	sigemptyset(&sa.sa_mask);
	NEED(sigaction(SIGALRM, &sa, NULL) == 0, "sigaction failed");
	(void)signal(SIGCHLD, SIG_DFL);
	(void)signal(SIGPIPE, SIG_IGN);
	alarm(secs);
}

int reap(pid_t k)
{
	int st;

	while (waitpid(k, &st, 0) == -1)
		if (errno != EINTR)
			return -1;
	return WIFEXITED(st) ? WEXITSTATUS(st) : -1;
}

int wait_fd(int fd, short ev, int ms)
{
	struct pollfd p;
	int n;

	p.fd = fd;
	p.events = ev;
	p.revents = 0;
	do
		n = poll(&p, 1, ms);
	while (n == -1 && errno == EINTR);
	NEED(n != -1, "poll failed: %s", pc_errname(errno));
	return n > 0 ? p.revents : 0;
}

/* Reads up to n bytes, waiting at most 5 s for each piece. Result: bytes read; *st: 0 full, 1 end of file, 2 timeout, 3 error. */
long rd(int fd, char *buf, size_t n, int *st)
{
	size_t got = 0;

	*st = 0;
	while (got < n) {
		ssize_t r;

		if (!wait_fd(fd, POLLIN, 5000)) {
			*st = 2;
			break;
		}
		r = read(fd, buf + got, n - got);
		if (r == 0) {
			*st = 1;
			break;
		}
		if (r < 0) {
			if (errno == EINTR)
				continue;
			*st = 3;
			break;
		}
		got += (size_t)r;
	}
	return (long)got;
}

void wr(int fd, const char *s)
{
	size_t n = strlen(s);

	NEED(write(fd, s, n) == (ssize_t)n, "write failed: %s", pc_errname(errno));
}

/* Exactly the bytes of s arrive on fd, in order. */
int arrives(int fd, const char *s)
{
	char buf[64];
	int st;
	size_t n = strlen(s);

	return rd(fd, buf, n, &st) == (long)n && memcmp(buf, s, n) == 0;
}

/* The next read on fd reports end-of-file (a return of 0) within 5 s. */
int at_eof(int fd)
{
	char c;
	ssize_t r;

	if (!wait_fd(fd, POLLIN, 5000))
		return 0;
	r = read(fd, &c, 1);
	return r == 0;
}

int main(void)
{
	int sv[2];

	pc_begin("XSH.shutdown.001");
	guard(30);
	NEED(socketpair(AF_UNIX, SOCK_STREAM, 0, sv) == 0, "socketpair failed: %s", pc_errname(errno));
	wr(sv[0], "hello");
	pc_test();
	pc_check(shutdown(sv[0], SHUT_WR) == 0, "shutdown(SHUT_WR) failed: %s", pc_errname(errno));
	pc_check(arrives(sv[1], "hello"), "data written before the shutdown did not arrive");
	NEED(at_eof(sv[1]), "the peer sees no end-of-file after SHUT_WR");
	wr(sv[1], "back");
	pc_check(arrives(sv[0], "back"), "the shut-down end no longer receives");
	pc_check(fcntl(sv[0], F_GETFD) != -1, "the descriptor is not open after shutdown()");
	pc_pass();
}
