/*
 * getpeername() returns the listener's name on the client and the client's own name on the accepted socket.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getpeername.002
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

void a4(struct sockaddr_in *a, unsigned port)
{
	memset(a, 0, sizeof *a);
	a->sin_family = AF_INET;
	a->sin_port = htons((in_port_t)port);
	a->sin_addr.s_addr = htonl(INADDR_LOOPBACK);
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

int main(void)
{
	struct sockaddr_in g, cn, pa, p1, p2;
	socklen_t cl = sizeof cn, pl = sizeof pa, l1 = sizeof p1, l2 = sizeof p2;
	int l, c, k;

	pc_begin("XSH.getpeername.002");
	guard(30);
	l = tlisten(&g);
	c = tconnect(&g);
	memset(&cn, 0, sizeof cn);
	NEED(getsockname(c, (struct sockaddr *)&cn, &cl) == 0, "getsockname failed: %s", pc_errname(errno));
	k = accept(l, (struct sockaddr *)&pa, &pl);
	NEED(k >= 0, "accept failed: %s", pc_errname(errno));
	memset(&p1, 0, sizeof p1);
	memset(&p2, 0, sizeof p2);
	pc_test();
	pc_check(getpeername(c, (struct sockaddr *)&p1, &l1) == 0, "getpeername() on the client failed: %s", pc_errname(errno));
	pc_check(p1.sin_family == AF_INET && p1.sin_port == g.sin_port && p1.sin_addr.s_addr == g.sin_addr.s_addr, "the client's peer is not the listener's address");
	pc_check(getpeername(k, (struct sockaddr *)&p2, &l2) == 0, "getpeername() on the accepted socket failed: %s", pc_errname(errno));
	pc_check(p2.sin_family == AF_INET && p2.sin_port == cn.sin_port && p2.sin_addr.s_addr == cn.sin_addr.s_addr, "the accepted socket's peer is not the client's own address");
	pc_pass();
}
