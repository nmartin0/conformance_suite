/*
 * getppid() returns the creator's process ID.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getppid.001
 * MUTANT    getppid.cached
 * MUTANT    getppid.self
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

/* The child: report getppid(), then create a grandchild that reports its own. */
static void child(void)
{
	long mine = (long)getppid();
	pid_t g = fork();

	if (g == -1) {
		report(-1, -1, 0);	/* both reports, so the test does not wait */
		report(-1, -1, 0);
		return;
	}
	if (g == 0) {
		report((long)getppid(), 0, 0);
		linger();
	}
	report(mine, (long)g, 0);
}

int main(void)
{
	pid_t me, kid;
	long c[3], g[3];

	pc_begin("XSH.getppid.001");
	pipes_init();
	me = getpid();
	pc_test();
	kid = spawn(child);
	/* The two reports may come in either order: the child's has the grandchild's ID. */
	get_report(c);
	get_report(g);
	if (g[1] != 0 && g[1] != -1) {
		long t[3];

		memcpy(t, c, sizeof t);
		memcpy(c, g, sizeof t);
		memcpy(g, t, sizeof t);
	}
	if (c[1] == -1)
		pc_unresolved("the child could not fork");
	pc_check(c[0] == (long)me, "the child's getppid() returned %ld, the test process is %ld", c[0], (long)me);
	pc_check(g[0] == (long)kid, "the grandchild's getppid() returned %ld, its creator is %ld", g[0], (long)kid);
	pc_pass();
}
