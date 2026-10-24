/*
 * A function registered by an exit handler is called.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.exit.004
 * MUTANT    exit.late_registration_skipped
 * MUTANT    exit.late_registration_last
 * MUTANT    exit.forward_order
 * VARIANT   wait.status_encoding
 * VARIANT   exit.late_atexit_fails
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

/* Waits for pid and returns its raw status; anything odd is UNRESOLVED. */
static int reap(pid_t pid)
{
	int st;

	if (waitpid(pid, &st, 0) != pid)
		pc_unresolved("waitpid failed");
	if (WIFEXITED(st) && WEXITSTATUS(st) == 99)
		pc_unresolved("the child could not set up");
	return st;
}

static pid_t spawn(void)
{
	pid_t r = fork();

	if (r == -1)
		pc_unresolved("fork failed");
	return r;
}

/* The child's exit handlers record their names here, in call order. */
static int trace_fd = -1;

static void mark(const char *s)
{
	(void)write(trace_fd, s, strlen(s));
}

/* Reads what the child recorded, up to end of file. */
static void collect(int fd, char *buf, size_t size)
{
	size_t n = 0;
	ssize_t r;

	while (n < size - 1 && (r = read(fd, buf + n, size - 1 - n)) > 0)
		n += (size_t)r;
	buf[n] = '\0';
}

static void b(void) { mark("b"); }
static void x(void) { mark("x"); }

static void a(void)
{
	mark("a");
	if (atexit(b) != 0)
		mark("!");
}

int main(void)
{
	int p[2], st;
	pid_t c;
	char buf[16];

	pc_begin("XSH.exit.004");
	pc_check(pipe(p) == 0, "pipe failed");
	pc_test();
	c = spawn();
	if (c == 0) {
		trace_fd = p[1];
		(void)close(p[0]);
		if (atexit(x) != 0 || atexit(a) != 0)
			_exit(99);
		exit(0);
	}
	(void)close(p[1]);
	st = reap(c);
	collect(p[0], buf, sizeof buf);
	pc_check(WIFEXITED(st) && WEXITSTATUS(st) == 0, "the child failed (%#x)", st);
	if (strchr(buf, '!') != NULL)
		pc_unresolved("atexit() failed inside an exit handler, which atexit() allows");
	pc_check(strcmp(buf, "abx") == 0, "handlers called as \"%s\", expected \"abx\"", buf);
	pc_pass();
}
