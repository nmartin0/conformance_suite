/*
 * A too short address buffer receives a truncated address while address_len reports the full length.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.accept.007
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

int main(void)
{
	struct sockaddr_in g, cn;
	socklen_t cl = sizeof cn, pl;
	unsigned char buf[32];
	size_t part = offsetof(struct sockaddr_in, sin_addr), i;
	int l, c, k;

	pc_begin("XSH.accept.007");
	guard(30);
	l = tlisten(&g);
	c = tconnect(&g);
	memset(&cn, 0, sizeof cn);
	NEED(getsockname(c, (struct sockaddr *)&cn, &cl) == 0, "getsockname failed: %s", pc_errname(errno));
	NEED(cl > part && sizeof buf > cl, "unexpected address length %lu", (unsigned long)cl);
	memset(buf, 0xAA, sizeof buf);
	pl = (socklen_t)part;
	pc_test();
	k = accept(l, (struct sockaddr *)buf, &pl);
	pc_check(k >= 0, "accept() with a short buffer failed: %s", pc_errname(errno));
	pc_check(pl == cl, "address_len is %lu after truncation, expected the full length %lu", (unsigned long)pl, (unsigned long)cl);
	pc_check(memcmp(buf, &cn, part) == 0, "the first %lu bytes are not the start of the address", (unsigned long)part);
	for (i = part; i < sizeof buf; i++)
		pc_check(buf[i] == 0xAA, "byte %lu beyond the supplied length was written", (unsigned long)i);
	close(k);
	c = tconnect(&g);
	memset(buf, 0xAA, sizeof buf);
	pl = 0;
	k = accept(l, (struct sockaddr *)buf, &pl);
	pc_check(k >= 0, "accept() with a zero length failed: %s", pc_errname(errno));
	pc_check(pl > 0, "address_len stayed 0 although the address has a length");
	for (i = 0; i < sizeof buf; i++)
		pc_check(buf[i] == 0xAA, "byte %lu was written although the supplied length was 0", (unsigned long)i);
	(void)c;
	pc_pass();
}
