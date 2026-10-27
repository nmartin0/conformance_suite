/*
 * Both descriptors of socketpair() are the lowest free ones when allocated.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.socketpair.004
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

/* Leaves slot L free, L+1 open and L+2 free; returns L. */
int make_holes(void)
{
	int lo, fd, mid;

	pc_make_file("holef", "x");
	lo = pc_lowest_free_fd();
	fd = open("holef", O_RDONLY);
	NEED(fd == lo, "cannot fill slot %d", lo);
	mid = fcntl(fd, F_DUPFD, lo + 1);
	NEED(mid == lo + 1, "cannot fill slot %d", lo + 1);
	NEED(close(fd) == 0, "close failed");
	NEED(fcntl(lo + 2, F_GETFD) == -1, "slot %d is not free", lo + 2);
	return lo;
}

int main(void)
{
	int sv[2], lo;

	pc_begin("XSH.socketpair.004");
	guard(30);
	lo = make_holes();
	pc_test();
	pc_check(socketpair(AF_UNIX, SOCK_STREAM, 0, sv) == 0, "socketpair() failed: %s", pc_errname(errno));
	pc_check((sv[0] == lo && sv[1] == lo + 2) || (sv[0] == lo + 2 && sv[1] == lo), "got %d and %d, expected %d and %d", sv[0], sv[1], lo, lo + 2);
	pc_pass();
}
