/*
 * getpgrp() returns the caller's process group ID.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getpgrp.001
 * MUTANT    getpgrp.cached
 * MUTANT    getpgrp.pid
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

static pid_t leader;

/* First child: getpgrp(), setpgid(0, 0), getpgrp(). */
static void first(void)
{
	long before = (long)getpgrp();
	int r = setpgid(0, 0);

	report(before, r == 0 ? 1 : 0, (long)getpgrp());
}

/* Second child: join the first child's group, then getpgrp(). */
static void second(void)
{
	int r = setpgid(0, leader);

	report((long)getpgrp(), r == 0 ? 1 : 0, 0);
}

int main(void)
{
	pid_t own;
	long v[3];

	pc_begin("XSH.getpgrp.001");
	pipes_init();
	own = getpgid(0);
	if (own <= 0)
		pc_unresolved("getpgid(0) failed: %s", pc_errname(errno));
	pc_test();
	leader = spawn(first);
	get_report(v);
	if (v[1] != 1)
		pc_unresolved("setpgid(0, 0) failed in the child");
	pc_check(v[0] == (long)own, "the child's getpgrp() returned %ld before setpgid(), its group is %ld",
		 v[0], (long)own);
	pc_check(v[2] == (long)leader, "getpgrp() returned %ld after setpgid(0, 0), expected %ld", v[2], (long)leader);
	(void)spawn(second);
	get_report(v);
	if (v[1] != 1)
		pc_unresolved("setpgid(0, %ld) failed in the second child", (long)leader);
	pc_check(v[0] == (long)leader, "getpgrp() in a member of group %ld returned %ld", (long)leader, v[0]);
	pc_pass();
}
