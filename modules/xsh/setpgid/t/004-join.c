/*
 * setpgid() moves a process into an existing group of the session.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.setpgid.004
 * MUTANT    setpgid.noop
 * MUTANT    setpgid.new_group
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

static pid_t c1;

static void lead(void)
{
	report(setpgid(0, 0) == 0 ? 1 : 0, 0, 0);
}

static void join(void)
{
	report(result((long)setpgid(0, c1)), 0, 0);
}

static void idle(void)
{
}

int main(void)
{
	pid_t c2, c3, g;
	long v[3];
	int r;

	pc_begin("XSH.setpgid.004");
	pipes_init();
	c1 = spawn(lead);
	get_report(v);
	if (v[0] != 1)
		pc_unresolved("setpgid(0, 0) failed in the first child");
	c3 = spawn(idle);
	pc_test();
	c2 = spawn(join);
	get_report(v);
	pc_check(v[0] == 0, "setpgid(0, c1) in c2 failed with %s", ename(v[0]));
	g = getpgid(c2);
	pc_check(g == c1, "after setpgid(0, c1), getpgid(c2) is %ld, expected %ld", (long)g, (long)c1);
	r = setpgid(c3, c1);
	pc_check(r == 0, "setpgid(c3, c1) failed with %s", pc_errname(errno));
	g = getpgid(c3);
	pc_check(g == c1, "after setpgid(c3, c1), getpgid(c3) is %ld, expected %ld", (long)g, (long)c1);
	pc_pass();
}
