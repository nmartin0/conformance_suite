/*
 * stdout and stderr write to descriptors 1 and 2.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.stdin.006
 */
#include "pc.h"
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>



static void on_guard(int sig)
{
	(void)sig;
	pc_fail("blocked: a child did not finish in time");
}

static void guard(unsigned secs)
{
	struct sigaction sa;

	memset(&sa, 0, sizeof sa);
	sa.sa_handler = on_guard;
	sigemptyset(&sa.sa_mask);
	pc_check(sigaction(SIGALRM, &sa, NULL) == 0, "sigaction failed");
	alarm(secs);
}

/* Reads until end-of-file or n-1 bytes; returns the count. */
static size_t slurp(int fd, char *buf, size_t n)
{
	size_t got = 0;
	ssize_t k;

	while (got < n - 1) {
		k = read(fd, buf + got, n - 1 - got);
		if (k > 0)
			got += (size_t)k;
		else if (k == 0)
			break;
		else if (errno != EINTR)
			pc_unresolved("read failed: %s", pc_errname(errno));
	}
	buf[got] = '\0';
	return got;
}

static int reap(pid_t c)
{
	int st;

	while (waitpid(c, &st, 0) == -1)
		if (errno != EINTR)
			pc_unresolved("waitpid failed: %s", pc_errname(errno));
	return st;
}

int main(void)
{
	char o[64], e[64];
	int po[2], pe[2], st;
	pid_t c;

	pc_begin("XSH.stdin.006");
	guard(30);
	pc_check(pipe(po) == 0 && pipe(pe) == 0, "pipe failed");
	pc_test();
	c = fork();
	if (c == -1)
		pc_unresolved("fork failed: %s", pc_errname(errno));
	if (c == 0) {
		(void)close(po[0]);
		(void)close(pe[0]);
		if (dup2(po[1], 1) != 1 || dup2(pe[1], 2) != 2)
			_exit(99);
		(void)fputs("to-out", stdout);
		(void)fputs("to-err", stderr);
		(void)fflush(stdout);
		(void)fflush(stderr);
		_exit(0);
	}
	(void)close(po[1]);
	(void)close(pe[1]);
	(void)slurp(po[0], o, sizeof o);
	(void)slurp(pe[0], e, sizeof e);
	(void)close(po[0]);
	(void)close(pe[0]);
	st = reap(c);
	pc_check(WIFEXITED(st) && WEXITSTATUS(st) == 0, "child status %#x", st);
	pc_check(strcmp(o, "to-out") == 0, "descriptor 1 received \"%s\"", o);
	pc_check(strcmp(e, "to-err") == 0, "descriptor 2 received \"%s\"", e);
	pc_pass();
}
