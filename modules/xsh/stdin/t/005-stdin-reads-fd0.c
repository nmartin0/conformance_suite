/*
 * stdin reads descriptor 0.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.stdin.005
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
	int p[2], st;
	pid_t c;

	pc_begin("XSH.stdin.005");
	guard(30);
	pc_check(pipe(p) == 0, "pipe failed");
	pc_test();
	c = fork();
	if (c == -1)
		pc_unresolved("fork failed: %s", pc_errname(errno));
	if (c == 0) {
		char line[64];

		(void)close(p[1]);
		if (dup2(p[0], 0) != 0)
			_exit(99);
		if (fgets(line, sizeof line, stdin) == NULL)
			_exit(10);
		if (strcmp(line, "hello stdin\n") != 0)
			_exit(11);
		if (fgets(line, sizeof line, stdin) != NULL)
			_exit(12);
		if (!feof(stdin))
			_exit(13);
		_exit(0);
	}
	(void)close(p[0]);
	{
		ssize_t w = write(p[1], "hello stdin\n", 12);

		pc_check(w == 12, "write to the child failed");
	}
	(void)close(p[1]);
	st = reap(c);
	pc_check(WIFEXITED(st) && WEXITSTATUS(st) == 0, "child status %#x (10: no line, 11: wrong text, 12: more data, 13: no end-of-file)", st);
	pc_pass();
}
