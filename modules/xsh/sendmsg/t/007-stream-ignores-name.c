/*
 * sendmsg() on a connected stream socket ignores msg_name (or fails with EISCONN).
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sendmsg.007
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
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/un.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

/* After pc_test(): a step that is not under test gives UNRESOLVED, not FAIL. */
#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)
#define MS 1000000LL

/* A call that blocks must not hang the test. Also gives SIGCHLD its default action so children can be waited for. */
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

static void nap_ms(long ms)
{
	struct timespec ts;

	ts.tv_sec = ms / 1000;
	ts.tv_nsec = (ms % 1000) * 1000000L;
	while (nanosleep(&ts, &ts) == -1 && errno == EINTR)
		;
}

static int again(int e)
{
	return e == EAGAIN || e == EWOULDBLOCK;
}

/* Test bytes that depend on position and seed. */
static void fillpat(unsigned char *b, size_t n, unsigned seed)
{
	size_t i;

	for (i = 0; i < n; i++)
		b[i] = (unsigned char)(seed + i * 7 + i / 251);
}

static void sockpair(int type, int sv[2])
{
	NEED(socketpair(AF_UNIX, type, 0, sv) == 0, "socketpair failed: %s", pc_errname(errno));
}

static void setnb(int fd, int on)
{
	int fl = fcntl(fd, F_GETFL);

	NEED(fl != -1, "F_GETFL failed: %s", pc_errname(errno));
	NEED(fcntl(fd, F_SETFL, on ? (fl | O_NONBLOCK) : (fl & ~O_NONBLOCK)) == 0, "F_SETFL failed: %s", pc_errname(errno));
}

/* Fills a AF_UNIX address for path; returns the length a caller passes. */
static socklen_t unix_name(struct sockaddr_un *a, const char *path)
{
	memset(a, 0, sizeof *a);
	a->sun_family = AF_UNIX;
	NEED(strlen(path) < sizeof a->sun_path, "socket path too long");
	strcpy(a->sun_path, path);
	return (socklen_t)(offsetof(struct sockaddr_un, sun_path) + strlen(path) + 1);
}

/* A datagram socket, bound to path in the current directory unless path is null. */
static int unix_dgram(const char *path)
{
	struct sockaddr_un a;
	socklen_t l;
	int s = socket(AF_UNIX, SOCK_DGRAM, 0);

	NEED(s != -1, "socket(AF_UNIX, SOCK_DGRAM) failed: %s", pc_errname(errno));
	if (path != NULL) {
		l = unix_name(&a, path);
		(void)unlink(path);
		NEED(bind(s, (struct sockaddr *)&a, l) == 0, "bind failed: %s", pc_errname(errno));
	}
	return s;
}

/* A loopback socket of the given type bound to an ephemeral port; *a gets its address. */
static int inet_sock(int type, struct sockaddr_in *a)
{
	socklen_t l = sizeof *a;
	int s = socket(AF_INET, type, 0);

	NEED(s != -1, "socket(AF_INET) failed: %s", pc_errname(errno));
	memset(a, 0, sizeof *a);
	a->sin_family = AF_INET;
	a->sin_addr.s_addr = pc_loopback4();
	NEED(bind(s, (struct sockaddr *)a, sizeof *a) == 0, "bind failed: %s", pc_errname(errno));
	NEED(getsockname(s, (struct sockaddr *)a, &l) == 0, "getsockname failed: %s", pc_errname(errno));
	return s;
}

/* A connected pair of loopback TCP sockets. */
static void tcp_pair(int out[2])
{
	struct sockaddr_in a;
	int l = inet_sock(SOCK_STREAM, &a), c, s;

	NEED(listen(l, 1) == 0, "listen failed: %s", pc_errname(errno));
	c = socket(AF_INET, SOCK_STREAM, 0);
	NEED(c != -1, "socket failed: %s", pc_errname(errno));
	NEED(connect(c, (struct sockaddr *)&a, sizeof a) == 0, "connect failed: %s", pc_errname(errno));
	s = accept(l, NULL, NULL);
	NEED(s != -1, "accept failed: %s", pc_errname(errno));
	close(l);
	out[0] = c;
	out[1] = s;
}

/* Greater than 0 once fd is readable within ms. */
static int wait_in(int fd, int ms)
{
	struct pollfd p;

	p.fd = fd; p.events = POLLIN; p.revents = 0;
	return poll(&p, 1, ms);
}

/* Receives up to n bytes from fd, waiting at most 5 s for each piece; returns the count. */
static size_t recv_n(int fd, void *buf, size_t n)
{
	size_t got = 0;
	ssize_t r;

	while (got < n) {
		if (wait_in(fd, 5000) <= 0)
			break;
		r = recv(fd, (char *)buf + got, n - got, 0);
		if (r <= 0)
			break;
		got += (size_t)r;
	}
	return got;
}

static void block_pipe(void)
{
	sigset_t s;

	sigemptyset(&s);
	sigaddset(&s, SIGPIPE);
	NEED(sigprocmask(SIG_BLOCK, &s, NULL) == 0, "sigprocmask failed: %s", pc_errname(errno));
}

static int pipe_pending(void)
{
	sigset_t s;

	sigemptyset(&s);
	NEED(sigpending(&s) == 0, "sigpending failed: %s", pc_errname(errno));
	return sigismember(&s, SIGPIPE) == 1;
}

/* Fills the send direction of a connected stream socket until it refuses (non-blocking); leaves O_NONBLOCK set. */
static void fill_stream(int fd)
{
	static char buf[4096];
	size_t total = 0;
	ssize_t r;

	memset(buf, 'f', sizeof buf);
	setnb(fd, 1);
	for (;;) {
		r = send(fd, buf, sizeof buf, MSG_NOSIGNAL);
		if (r == -1) {
			NEED(again(errno), "send while filling failed: %s", pc_errname(errno));
			break;
		}
		total += (size_t)r;
		NEED(total < (64u << 20), "64 MiB were accepted without the socket refusing");
	}
	for (;;) {
		r = send(fd, buf, 1, MSG_NOSIGNAL);
		if (r == -1) {
			NEED(again(errno), "send while filling failed: %s", pc_errname(errno));
			break;
		}
		total += (size_t)r;
		NEED(total < (64u << 20), "64 MiB were accepted without the socket refusing");
	}
}

/* Forks a child that runs body-less: returns the pid in the parent, 0 in the child. */
static pid_t spawn(void)
{
	pid_t p = fork();

	NEED(p != -1, "fork failed: %s", pc_errname(errno));
	return p;
}

static void reap(pid_t p)
{
	int st;

	while (waitpid(p, &st, 0) == -1 && errno == EINTR)
		;
}

/* Sets up a msghdr with a name and a vector (no control data). */
static void mhdr(struct msghdr *m, void *name, socklen_t nl, struct iovec *iov, int iovlen)
{
	memset(m, 0, sizeof *m);
	m->msg_name = name;
	m->msg_namelen = nl;
	m->msg_iov = iov;
	m->msg_iovlen = iovlen;
}

static long iov_max(void)
{
#ifdef IOV_MAX
	return IOV_MAX;
#else
	return sysconf(_SC_IOV_MAX);
#endif
}


int main(void)
{
	struct sockaddr_un a;
	socklen_t l;
	struct msghdr m;
	struct iovec iov[1];
	int sv[2];
	ssize_t n;
	char c = 0;

	pc_begin("XSH.sendmsg.007");
	guard(30);
	sockpair(SOCK_STREAM, sv);
	l = unix_name(&a, "nobody.sock");
	iov[0].iov_base = "k"; iov[0].iov_len = 1;
	mhdr(&m, &a, l, iov, 1);
	pc_test();
	errno = 0;
	n = sendmsg(sv[0], &m, 0);
	if (n == -1) {
		pc_check(errno == EISCONN, "sendmsg failed with %s, expected success or EISCONN", pc_errname(errno));
		pc_check(wait_in(sv[1], 300) == 0, "sendmsg failed with EISCONN but a byte arrived");
		pc_pass();
	}
	pc_check(n == 1, "sendmsg returned %ld, expected 1 or -1", (long)n);
	NEED(wait_in(sv[1], 5000) > 0, "the byte did not arrive");
	n = recv(sv[1], &c, 1, 0);
	pc_check(n == 1 && c == 'k', "the peer received %ld byte(s), first %d; expected 'k'", (long)n, (int)c);
	pc_pass();
}
