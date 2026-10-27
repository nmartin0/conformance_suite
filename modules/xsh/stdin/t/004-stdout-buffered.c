/*
 * stdout is fully buffered on a pipe.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.stdin.004
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

#include <fcntl.h>
#include <poll.h>

int main(void)
{
	char buf[64], sync;
	int po[2], up[2], down[2], st;
	pid_t c;
	size_t n;
	struct pollfd pf;
	int ready;

	pc_begin("XSH.stdin.004");
	guard(30);
	pc_check(pipe(po) == 0 && pipe(up) == 0 && pipe(down) == 0, "pipe failed");
	pc_test();
	c = fork();
	if (c == -1)
		pc_unresolved("fork failed: %s", pc_errname(errno));
	if (c == 0) {
		(void)close(po[0]);
		(void)close(up[0]);
		(void)close(down[1]);
		if (dup2(po[1], 1) != 1)
			_exit(99);
		(void)fputs("buffered\n", stdout);
		if (write(up[1], "s", 1) != 1)
			_exit(98);
		if (read(down[0], &sync, 1) != 1)
			_exit(97);
		(void)fflush(stdout);
		_exit(0);
	}
	(void)close(po[1]);
	(void)close(up[1]);
	(void)close(down[0]);
	n = (size_t)read(up[0], &sync, 1);
	pc_check(n == 1, "the child did not report");
	pf.fd = po[0];
	pf.events = POLLIN;
	pf.revents = 0;
	ready = poll(&pf, 1, 300);
	pc_check(ready == 0, "data had reached the pipe before fflush(): stdout is not fully buffered");
	pc_check(write(down[1], "g", 1) == 1, "write to the child failed");
	n = slurp(po[0], buf, sizeof buf);
	(void)close(po[0]);
	(void)close(up[0]);
	(void)close(down[1]);
	st = reap(c);
	pc_check(WIFEXITED(st) && WEXITSTATUS(st) == 0, "child status %#x", st);
	pc_check(strcmp(buf, "buffered\n") == 0, "after fflush() the pipe holds \"%s\"", buf);
	pc_pass();
}
