/*
 * quick_exit() ends the process with the given status.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.quick_exit.003
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

static void h(void) { put("h", 1); }

int main(void)
{
	static const int codes[] = { 0, 1, 42, 255 };
	size_t k;
	int with;

	pc_begin("XSH.quick_exit.003");
	guard(60);
	pc_test();
	for (with = 0; with < 2; with++)
		for (k = 0; k < sizeof codes / sizeof codes[0]; k++) {
			int p[2], st;
			pid_t c;
			char buf[8];

			if (pipe(p) != 0)
				pc_unresolved("pipe failed");
			c = fork();
			if (c == -1)
				pc_unresolved("fork failed: %s", pc_errname(errno));
			if (c == 0) {
				(void)close(p[0]);
				rfd = p[1];
				if (with && at_quick_exit(h) != 0)
					_exit(99);
				quick_exit(codes[k]);
				_exit(98);
			}
			(void)close(p[1]);
			(void)slurp(p[0], buf, sizeof buf);
			(void)close(p[0]);
			st = reap(c);
			pc_check(WIFEXITED(st) && WEXITSTATUS(st) == codes[k], "quick_exit(%d): child status %#x", codes[k], st);
		}
	pc_pass();
}
