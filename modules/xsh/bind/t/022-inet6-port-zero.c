/*
 * Binding port 0 on the IPv6 loopback address chooses a real port.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.bind.022
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

int main(void)
{
	struct sockaddr_in6 a, g;
	socklen_t l = sizeof g;
	int s, r;

	pc_begin("XSH.bind.022");
	guard(30);
	pc_test();
	s = socket(AF_INET6, SOCK_STREAM, 0);
	if (s == -1 && errno == EAFNOSUPPORT)
		pc_notinuse("AF_INET6 is not supported here");
	NEED(s >= 0, "socket(AF_INET6) failed: %s", pc_errname(errno));
	memset(&a, 0, sizeof a);
	a.sin6_family = AF_INET6;
	a.sin6_addr = in6addr_loopback;
	r = bind(s, (struct sockaddr *)&a, sizeof a);
	if (r == -1 && errno == EADDRNOTAVAIL)
		pc_unresolved("no IPv6 loopback address on this host");
	pc_check(r == 0, "bind() to the IPv6 loopback address, port 0, failed: %s", pc_errname(errno));
	memset(&g, 0, sizeof g);
	NEED(getsockname(s, (struct sockaddr *)&g, &l) == 0, "getsockname failed: %s", pc_errname(errno));
	pc_check(g.sin6_family == AF_INET6, "family is %d, expected AF_INET6", (int)g.sin6_family);
	pc_check(ntohs(g.sin6_port) != 0, "the chosen port is 0");
	close(s);
	pc_pass();
}
