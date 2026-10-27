/*
 * SO_SNDTIMEO can be set and read back (possibly rounded up).
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.setsockopt.010
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
	a->sin_addr.s_addr = htonl(INADDR_LOOPBACK);
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

/* Sends one data byte and n (1 to 4) descriptors in an SCM_RIGHTS message; a failure is a setup failure. */
static void send_fds(int sock, const int *fds, int n, char data)
{
	union { struct cmsghdr h; char b[CMSG_SPACE(4 * sizeof(int))]; } u;
	struct msghdr m;
	struct iovec iov;
	struct cmsghdr *c;

	NEED(n >= 1 && n <= 4, "bad descriptor count");
	memset(&u, 0, sizeof u);
	iov.iov_base = &data;
	iov.iov_len = 1;
	mhdr(&m, NULL, 0, &iov, 1);
	m.msg_control = u.b;
	m.msg_controllen = CMSG_SPACE((size_t)n * sizeof(int));
	c = CMSG_FIRSTHDR(&m);
	NEED(c != NULL, "CMSG_FIRSTHDR returned a null pointer");
	c->cmsg_level = SOL_SOCKET;
	c->cmsg_type = SCM_RIGHTS;
	c->cmsg_len = CMSG_LEN((size_t)n * sizeof(int));
	memcpy(CMSG_DATA(c), fds, (size_t)n * sizeof(int));
	NEED(sendmsg(sock, &m, 0) == 1, "sendmsg with an SCM_RIGHTS message failed: %s", pc_errname(errno));
}

/* Sets a Boolean option to a non-zero value and to 0 and reads it back. ENOPROTOOPT means the protocol lacks it. */
static void boolrt(int s, int level, int opt, const char *name)
{
	int v = 5, g = -1;
	socklen_t l = sizeof g;

	if (setsockopt(s, level, opt, &v, sizeof v) == -1) {
		if (errno == ENOPROTOOPT)
			pc_notinuse("%s is not supported by this protocol", name);
		if (errno == ENOBUFS || errno == ENOMEM)
			pc_unresolved("setting %s failed with %s (a may-fail error)", name, pc_errname(errno));
		pc_fail("setting %s failed with %s", name, pc_errname(errno));
	}
	pc_check(getsockopt(s, level, opt, &g, &l) == 0, "reading %s failed: %s", name, pc_errname(errno));
	pc_check(g != 0, "%s was set to 5 and reads back %d, expected non-zero", name, g);
	v = 0; g = -1; l = sizeof g;
	pc_check(setsockopt(s, level, opt, &v, sizeof v) == 0, "setting %s to 0 failed: %s", name, pc_errname(errno));
	pc_check(getsockopt(s, level, opt, &g, &l) == 0, "reading %s failed: %s", name, pc_errname(errno));
	pc_check(g == 0, "%s was set to 0 and reads back %d", name, g);
}


int main(void)
{
	int sv[2];
	struct timeval tv, g;
	socklen_t l = sizeof g;

	pc_begin("XSH.setsockopt.010");
	guard(30);
	sockpair(SOCK_STREAM, sv);
	pc_test();
	tv.tv_sec = 1; tv.tv_usec = 500000;
	if (setsockopt(sv[0], SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof tv) == -1) {
		if (errno == ENOPROTOOPT || errno == EINVAL)
			pc_notinuse("SO_SNDTIMEO cannot be set here (%s); the page leaves that open", pc_errname(errno));
		pc_fail("setting SO_SNDTIMEO failed with %s", pc_errname(errno));
	}
	memset(&g, 0, sizeof g);
	pc_check(getsockopt(sv[0], SOL_SOCKET, SO_SNDTIMEO, &g, &l) == 0, "reading SO_SNDTIMEO failed: %s", pc_errname(errno));
	pc_check(l == sizeof g, "the option length is %lu, expected %lu", (unsigned long)l, (unsigned long)sizeof g);
	pc_check((long long)g.tv_sec * 1000000 + g.tv_usec >= 1500000, "SO_SNDTIMEO was set to 1.5 s and reads back {%ld, %ld}, smaller", (long)g.tv_sec, (long)g.tv_usec);
	pc_pass();
}
