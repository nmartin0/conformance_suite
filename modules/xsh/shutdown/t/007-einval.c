/*
 * shutdown() with a value that is none of SHUT_RD, SHUT_WR, SHUT_RDWR fails with EINVAL.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.shutdown.007
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
	int sv[2], bad[2], i, r, top = SHUT_RD;

	pc_begin("XSH.shutdown.007");
	guard(30);
	NEED(socketpair(AF_UNIX, SOCK_STREAM, 0, sv) == 0, "socketpair failed: %s", pc_errname(errno));
	if (SHUT_WR > top)
		top = SHUT_WR;
	if (SHUT_RDWR > top)
		top = SHUT_RDWR;
	bad[0] = top + 1000;
	bad[1] = (SHUT_RD != -1 && SHUT_WR != -1 && SHUT_RDWR != -1) ? -1 : top + 2000;
	pc_test();
	for (i = 0; i < 2; i++) {
		errno = 0;
		r = shutdown(sv[0], bad[i]);
		pc_check(r == -1 && errno == EINVAL, "shutdown(%d) gave %s, expected EINVAL", bad[i], en(r, errno));
	}
	pc_pass();
}
