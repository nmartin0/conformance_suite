/*
 * A refused non-blocking connect() reports ECONNREFUSED at once or through SO_ERROR, which is then cleared.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.connect.016
 */
#include "pc.h"
#include "pcnet.h"
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

void a4(struct sockaddr_in *a, unsigned port)
{
	memset(a, 0, sizeof *a);
	a->sin_family = AF_INET;
	a->sin_port = htons((in_port_t)port);
	a->sin_addr.s_addr = pc_loopback4();
}

int tsock(void)
{
	int s = socket(AF_INET, SOCK_STREAM, 0);

	NEED(s != -1, "socket(AF_INET) failed: %s", pc_errname(errno));
	return s;
}

/* A TCP socket bound to the loopback address with an ephemeral port; *got is its name. */
int tbind(struct sockaddr_in *got)
{
	struct sockaddr_in a;
	socklen_t l = sizeof *got;
	int s = tsock();

	a4(&a, 0);
	NEED(bind(s, (struct sockaddr *)&a, sizeof a) == 0, "bind failed: %s", pc_errname(errno));
	memset(got, 0, sizeof *got);
	NEED(getsockname(s, (struct sockaddr *)got, &l) == 0, "getsockname failed: %s", pc_errname(errno));
	return s;
}

int tlisten(struct sockaddr_in *got)
{
	int s = tbind(got);

	NEED(listen(s, 4) == 0, "listen failed: %s", pc_errname(errno));
	return s;
}

int tconnect(const struct sockaddr_in *to)
{
	int c = tsock();

	NEED(connect(c, (const struct sockaddr *)to, sizeof *to) == 0, "connect failed: %s", pc_errname(errno));
	return c;
}

/* A loopback port nobody listens on (bound, then closed). */
unsigned dead_port(void)
{
	struct sockaddr_in g;
	int s = tbind(&g);

	close(s);
	return ntohs(g.sin_port);
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
	struct sockaddr_in a;
	int c, r, e, err = -1, rev;
	socklen_t el = sizeof err;

	pc_begin("XSH.connect.016");
	guard(30);
	a4(&a, dead_port());
	c = tsock();
	NEED(fcntl(c, F_SETFL, O_NONBLOCK) == 0, "fcntl failed: %s", pc_errname(errno));
	pc_test();
	errno = 0;
	r = connect(c, (struct sockaddr *)&a, sizeof a);
	e = errno;
	pc_check(r == -1, "connect() to a port nobody listens on succeeded");
	if (e == ECONNREFUSED)
		pc_pass();
	pc_check(e == EINPROGRESS, "non-blocking connect() failed with %s, expected EINPROGRESS or ECONNREFUSED", pc_errname(e));
	rev = wait_fd(c, POLLOUT, 5000);
	if (!rev)
		pc_unresolved("no result of the connection attempt within 5 s");
	pc_check(getsockopt(c, SOL_SOCKET, SO_ERROR, &err, &el) == 0, "getsockopt(SO_ERROR) failed: %s", pc_errname(errno));
	pc_check(err == ECONNREFUSED, "SO_ERROR is %s, expected ECONNREFUSED", pc_errname(err));
	err = -1;
	el = sizeof err;
	pc_check(getsockopt(c, SOL_SOCKET, SO_ERROR, &err, &el) == 0, "second getsockopt(SO_ERROR) failed: %s", pc_errname(errno));
	pc_check(err == 0, "SO_ERROR is %s on the second read, expected the error to be cleared", pc_errname(err));
	pc_pass();
}
