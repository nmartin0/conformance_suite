/*
 * A registered function that does not return stops the rest.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.quick_exit.004
 */
#include "pc.h"
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

static int rfd = -1;	/* the child's end of the report pipe */

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

static void put(const char *s, size_t n)
{
	ssize_t w = write(rfd, s, n);

	(void)w;
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

static void first_registered(void) { put("a", 1); }
static void second_registered(void) { put("b", 1); _exit(33); }

int main(void)
{
	char buf[32];
	int p[2], st;
	pid_t c;
	size_t n;

	pc_begin("XSH.quick_exit.004");
	guard(30);
	pc_check(pipe(p) == 0, "pipe failed");
	pc_test();
	c = fork();
	if (c == -1)
		pc_unresolved("fork failed: %s", pc_errname(errno));
	if (c == 0) {
		(void)close(p[0]);
		rfd = p[1];
		if (at_quick_exit(first_registered) != 0 || at_quick_exit(second_registered) != 0)
			_exit(99);
		quick_exit(5);
		_exit(98);
	}
	(void)close(p[1]);
	n = slurp(p[0], buf, sizeof buf);
	(void)close(p[0]);
	st = reap(c);
	pc_check(WIFEXITED(st) && WEXITSTATUS(st) == 33, "child status %#x, expected the exit status 33 of the function", st);
	pc_check(n == 1 && buf[0] == 'b', "the report is \"%s\", expected only \"b\"", buf);
	pc_pass();
}
