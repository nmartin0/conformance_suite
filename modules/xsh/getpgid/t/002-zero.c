/*
 * getpgid(0) returns the caller's group.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getpgid.002
 * MUTANT    getpgid.zero_esrch
 * MUTANT    getpgid.zero_pid
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

/* A member of c1's group, created in the test phase: getpgid(0) after joining. */
static void join(void)
{
	int r = setpgid(0, c1);

	report(r == 0 ? 1 : 0, result((long)getpgid(0)), 0);
}

int main(void)
{
	long v[3];

	pc_begin("XSH.getpgid.002");
	pipes_init();
	c1 = spawn(lead);
	get_report(v);
	if (v[0] != 1)
		pc_unresolved("setpgid(0, 0) failed in the child");
	pc_test();
	(void)spawn(join);
	get_report(v);
	if (v[0] != 1)
		pc_unresolved("setpgid(0, c1) failed in the second child");
	pc_check(v[1] == (long)c1, "getpgid(0) in a member of group %ld returned %ld (%s)", (long)c1, v[1],
		 ename(v[1]));
	pc_pass();
}
