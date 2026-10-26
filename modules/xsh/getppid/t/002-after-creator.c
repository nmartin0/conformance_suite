/*
 * getppid() no longer names a creator whose status was collected.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getppid.002
 * MUTANT    getppid.cached
 */
#include "pc.h"
#include <errno.h>
#include <limits.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

/*
 * Helper processes report over rep[] and then wait on hold[0] until the
 * test process closes hold[1] or ends.
 */
static int hold[2], rep[2];

static void pipes_init(void)
{
	if (pipe(hold) != 0 || pipe(rep) != 0)
		pc_unresolved("pipe failed: %s", pc_errname(errno));
}

_Noreturn static void linger(void)
{
	char c;

	(void)close(hold[1]);
	while (read(hold[0], &c, 1) == -1 && errno == EINTR)
		;
	_exit(0);
}

/* A helper's report: three numbers; for a failed call, -1000 - errno. */
static void report(long a, long b, long c)
{
	long v[3];
	ssize_t w;

	v[0] = a;
	v[1] = b;
	v[2] = c;
	w = write(rep[1], v, sizeof v);
	(void)w;
}

static long result(long r)
{
	return r == -1 ? -1000 - errno : r;
}

/* Reads one report; UNRESOLVED (setup) or FAIL if a helper died first. */
static void get_report(long v[3])
{
	size_t got = 0;
	ssize_t n;

	while (got < 3 * sizeof(long)) {
		n = read(rep[0], (char *)v + got, 3 * sizeof(long) - got);
		if (n > 0)
			got += (size_t)n;
		else if (n == 0 || errno != EINTR)
			pc_unresolved("a helper process did not report");
	}
}

static pid_t spawn(void (*fn)(void))
{
	pid_t pid = fork();

	if (pid == -1)
		pc_unresolved("fork failed: %s", pc_errname(errno));
	if (pid == 0) {
		fn();
		linger();
	}
	return pid;
}

static const char *ename(long v)
{
	return v <= -1000 ? pc_errname((int)(-1000 - v)) : "no error";
}

static int go[2], ready[2];

/* The grandchild: report getppid(), wait for the word, report it again. */
static void grandchild(void)
{
	char c;
	long first = (long)getppid();
	ssize_t w = write(ready[1], "x", 1);

	(void)w;
	report(first, 1, 0);
	(void)close(go[1]);
	while (read(go[0], &c, 1) == -1 && errno == EINTR)
		;
	report((long)getppid(), 2, 0);
}

/* The child: create the grandchild, wait until it has called getppid(), then exit. */
static void child(void)
{
	char c;
	pid_t g = fork();

	if (g == -1)
		_exit(1);
	if (g == 0) {
		grandchild();
		linger();
	}
	while (read(ready[0], &c, 1) == -1 && errno == EINTR)
		;
	_exit(0);
}

int main(void)
{
	pid_t kid;
	long v[3];
	int st;
	ssize_t w;

	pc_begin("XSH.getppid.002");
	pipes_init();
	if (pipe(go) != 0 || pipe(ready) != 0)
		pc_unresolved("pipe failed");
	pc_test();
	kid = fork();
	if (kid == -1)
		pc_unresolved("fork failed: %s", pc_errname(errno));
	if (kid == 0)
		child();
	get_report(v);
	if (v[1] != 1)
		pc_unresolved("unexpected report");
	if (v[0] != (long)kid)
		pc_unresolved("the grandchild's parent is %ld before the child ends, not %ld (XSH.getppid.001)",
			      v[0], (long)kid);
	while (waitpid(kid, &st, 0) == -1)
		if (errno != EINTR)
			pc_unresolved("waitpid failed: %s", pc_errname(errno));
	if (!WIFEXITED(st) || WEXITSTATUS(st) != 0)
		pc_unresolved("the child failed (status %#x)", (unsigned)st);
	w = write(go[1], "x", 1);
	if (w != 1)
		pc_unresolved("cannot wake the grandchild");
	get_report(v);
	pc_check(v[0] != (long)kid, "the grandchild's getppid() still returns %ld after its creator was reaped",
		 (long)kid);
	pc_check(v[0] > 0, "the grandchild's getppid() returned %ld", v[0]);
	pc_pass();
}
