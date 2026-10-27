/*
 * socket() with no free descriptor fails with EMFILE and allocates nothing.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.socket.015
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

/* Opens every free slot below the descriptor limit except the top nfree ones, then sets the limit there; returns the limit. */
int fill_to(int nfree)
{
	struct rlimit rl;
	int i, src, lim = 20;

	pc_make_file("fillf", "x");
	src = open("fillf", O_RDONLY);
	NEED(src >= 0, "open failed");
	for (i = 0; i < 1024; i++)
		if (fcntl(i, F_GETFD) != -1 && i + 1 + nfree > lim)
			lim = i + 1 + nfree;
	NEED(lim <= 256, "a descriptor near %d is open; too many to fill", lim);
	for (i = 0; i < lim - nfree; i++)
		if (fcntl(i, F_GETFD) == -1)
			NEED(dup2(src, i) == i, "dup2 to %d failed", i);
	NEED(getrlimit(RLIMIT_NOFILE, &rl) == 0, "getrlimit failed");
	rl.rlim_cur = (rlim_t)lim;
	NEED(setrlimit(RLIMIT_NOFILE, &rl) == 0, "setrlimit failed");
	NEED(getrlimit(RLIMIT_NOFILE, &rl) == 0 && rl.rlim_cur == (rlim_t)lim, "RLIMIT_NOFILE did not take the value %d", lim);
	return lim;
}

int main(void)
{
	int lim, s, i, open_after = 0;

	pc_begin("XSH.socket.015");
	guard(30);
	lim = fill_to(0);
	pc_test();
	errno = 0;
	s = socket(AF_UNIX, SOCK_STREAM, 0);
	pc_check(s == -1 && errno == EMFILE, "socket() with every descriptor in use gave %d (%s), expected EMFILE", s, s == -1 ? pc_errname(errno) : "success");
	for (i = 0; i < lim; i++)
		if (fcntl(i, F_GETFD) != -1)
			open_after++;
	pc_check(open_after == lim, "descriptors changed after the failed call");
	pc_pass();
}
