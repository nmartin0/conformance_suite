/*
 * Registrations that returned 0 are honoured, failed ones are not.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.atexit.005
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

/* A call that blocks must fail the test, not hang it. */
static void on_guard(int sig)
{
	(void)sig;
	pc_fail("blocked: the call under test did not finish in time");
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

/* Reads from fd until end-of-file or n bytes; returns the count. */
static size_t slurp(int fd, void *buf, size_t n)
{
	unsigned char *b = buf;
	size_t got = 0;
	ssize_t k;

	while (got < n) {
		k = read(fd, b + got, n - got);
		if (k > 0)
			got += (size_t)k;
		else if (k == 0)
			break;
		else if (errno != EINTR)
			pc_unresolved("read failed: %s", pc_errname(errno));
	}
	return got;
}

/* Writes bytes to fd, ignoring the result (used where nothing can be done about it). */
static void put(int fd, const void *p, size_t n)
{
	ssize_t w = write(fd, p, n);

	(void)w;
}

/* Waits for c; returns its wait status. */
static int reap(pid_t c)
{
	int st;

	while (waitpid(c, &st, 0) == -1)
		if (errno != EINTR)
			pc_unresolved("waitpid failed: %s", pc_errname(errno));
	return st;
}

static int rfd = -1;	/* the child's end of the report pipe */

/* Registered function: reports one byte. */
static void handler(void)
{
	put(rfd, "H", 1);
}

#define TRIES 4096

int main(void)
{
	static unsigned char buf[TRIES + 64];
	size_t n;
	unsigned long reported;
	int p[2], st;
	pid_t c;

	pc_begin("XSH.atexit.005");
	guard(30);
	pc_check(pipe(p) == 0, "pipe failed");
	pc_test();
	c = fork();
	if (c == -1)
		pc_unresolved("fork failed: %s", pc_errname(errno));
	if (c == 0) {
		unsigned char ok[4];
		unsigned long good = 0;
		int i;

		(void)close(p[0]);
		rfd = p[1];
		for (i = 0; i < TRIES; i++)
			if (atexit(handler) == 0)
				good++;
		ok[0] = (unsigned char)(good & 0xff);
		ok[1] = (unsigned char)((good >> 8) & 0xff);
		ok[2] = 0;
		ok[3] = 0;
		put(rfd, ok, 4);
		exit(0);
	}

	(void)close(p[1]);
	n = slurp(p[0], buf, sizeof buf);
	(void)close(p[0]);
	st = reap(c);

	pc_check(n >= 4, "the child reported %d bytes", (int)n);
	reported = buf[0] | ((unsigned long)buf[1] << 8);
	pc_check(reported >= 32, "only %lu of %d registrations returned 0", reported, TRIES);
	pc_check(n - 4 == reported, "%lu registrations returned 0 but the function ran %lu times", reported, (unsigned long)(n - 4));
	pc_check(WIFEXITED(st) && WEXITSTATUS(st) == 0, "child status %#x", st);
	pc_pass();
}
