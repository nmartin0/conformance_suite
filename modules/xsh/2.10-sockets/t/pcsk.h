/*
 * pcsk.h - helpers shared by the tests of XSH 2.10 (sockets).
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * Loopback addresses and AF_UNIX names in the working directory only.
 */
#ifndef PCSK_H
#define PCSK_H

#include "pc.h"
#include "pcnet.h"
#include "pcnet.h"
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <poll.h>
#include <signal.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/un.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

/* After pc_test(): a step that is not under test gives UNRESOLVED, not FAIL. */
#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

/* A call that blocks must not hang the test: the last resort is UNRESOLVED. */
static void pcsk_on_guard(int sig)
{
	(void)sig;
	pc_unresolved("a call did not finish within the guard time");
}

static inline void guard(unsigned secs)
{
	struct sigaction sa;

	memset(&sa, 0, sizeof sa);
	sa.sa_handler = pcsk_on_guard;
	sigemptyset(&sa.sa_mask);
	NEED(sigaction(SIGALRM, &sa, NULL) == 0, "sigaction failed");
	(void)signal(SIGCHLD, SIG_DFL);
	alarm(secs);
}

static inline long long mono_ms(void)
{
	struct timespec t;

	NEED(clock_gettime(CLOCK_MONOTONIC, &t) == 0, "clock_gettime failed: %s", pc_errname(errno));
	return (long long)t.tv_sec * 1000LL + t.tv_nsec / 1000000L;
}

static inline void nap_ms(long ms)
{
	struct timespec ts;

	ts.tv_sec = ms / 1000;
	ts.tv_nsec = (ms % 1000) * 1000000L;
	while (nanosleep(&ts, &ts) == -1 && errno == EINTR)
		;
}

static inline int again(int e)
{
	return e == EAGAIN || e == EWOULDBLOCK;
}

static inline void set_nb(int fd, int on)
{
	int fl = fcntl(fd, F_GETFL);

	NEED(fl != -1, "F_GETFL failed: %s", pc_errname(errno));
	NEED(fcntl(fd, F_SETFL, on ? (fl | O_NONBLOCK) : (fl & ~O_NONBLOCK)) == 0, "F_SETFL failed: %s", pc_errname(errno));
}

/* A name in the working directory for a socket; returns the length to pass. */
static inline socklen_t unix_name(struct sockaddr_un *a, const char *path)
{
	memset(a, 0, sizeof *a);
	a->sun_family = AF_UNIX;
	NEED(strlen(path) < sizeof a->sun_path, "socket path too long");
	strcpy(a->sun_path, path);
	return (socklen_t)(offsetof(struct sockaddr_un, sun_path) + strlen(path) + 1);
}

/* Greater than 0 once fd is ready for events within ms. */
static inline int wait_fd(int fd, short events, int ms)
{
	struct pollfd p;

	p.fd = fd;
	p.events = events;
	p.revents = 0;
	return poll(&p, 1, ms);
}

/* Receives up to n bytes from fd, waiting at most 5 s for each piece; returns the count. */
static inline size_t recv_n(int fd, void *buf, size_t n)
{
	size_t got = 0;
	ssize_t r;

	while (got < n) {
		if (wait_fd(fd, POLLIN, 5000) <= 0)
			break;
		r = recv(fd, (char *)buf + got, n - got, 0);
		if (r <= 0)
			break;
		got += (size_t)r;
	}
	return got;
}

/* A listening TCP socket on the IPv4 loopback with an ephemeral port; *a is its address. */
static inline int tcp_listener(struct sockaddr_in *a)
{
	socklen_t l = sizeof *a;
	int s = socket(AF_INET, SOCK_STREAM, 0);

	NEED(s != -1, "socket(AF_INET) failed: %s", pc_errname(errno));
	memset(a, 0, sizeof *a);
	a->sin_family = AF_INET;
	a->sin_addr.s_addr = pc_loopback4();
	NEED(bind(s, (struct sockaddr *)a, sizeof *a) == 0, "bind failed: %s", pc_errname(errno));
	NEED(listen(s, 5) == 0, "listen failed: %s", pc_errname(errno));
	NEED(getsockname(s, (struct sockaddr *)a, &l) == 0, "getsockname failed: %s", pc_errname(errno));
	return s;
}

/* A connected pair of loopback TCP sockets: out[0] the client, out[1] the accepted side. */
static inline void tcp_pair(int out[2])
{
	struct sockaddr_in a;
	int l = tcp_listener(&a), c, s;

	c = socket(AF_INET, SOCK_STREAM, 0);
	NEED(c != -1, "socket failed: %s", pc_errname(errno));
	NEED(connect(c, (struct sockaddr *)&a, sizeof a) == 0, "connect failed: %s", pc_errname(errno));
	NEED(wait_fd(l, POLLIN, 5000) > 0, "the connection did not arrive");
	s = accept(l, NULL, NULL);
	NEED(s != -1, "accept failed: %s", pc_errname(errno));
	(void)close(l);
	out[0] = c;
	out[1] = s;
}

/* A TCP port on the loopback address that nobody is listening on. */
static inline void closed_port(struct sockaddr_in *a)
{
	struct sockaddr_in l;
	int s = tcp_listener(&l);

	*a = l;
	(void)close(s);
}

static inline const char *en(int r, int e)
{
	return r == -1 ? pc_errname(e) : "success";
}

#endif
