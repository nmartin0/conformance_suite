/*
 * A socket type the address family does not support gives one of the listed errors.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.socket.012
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
	static const int cand[3] = { 9999, 777, 4242 };
	static const int dom[2] = { AF_UNIX, AF_INET };
	int i, d, s, e, tried = 0;

	pc_begin("XSH.socket.012");
	guard(30);
	pc_test();
	for (d = 0; d < 2; d++)
		for (i = 0; i < 3; i++) {
			errno = 0;
			s = socket(dom[d], cand[i], 0);
			e = errno;
			if (s >= 0) {
				close(s);	/* an extension type */
				continue;
			}
			tried++;
			/* EINVAL is tolerated: see the standard issue on invalid type values. */
			pc_check(is_err(e, ESOCKTNOSUPPORT, EPROTONOSUPPORT, EPROTOTYPE, EINVAL),
			    "socket(%d, %d, 0) failed with %s", dom[d], cand[i], pc_errname(e));
		}
	if (!tried)
		pc_unresolved("every candidate type value was accepted");
	pc_pass();
}
