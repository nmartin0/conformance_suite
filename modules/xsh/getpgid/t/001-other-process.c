/*
 * getpgid(pid) returns the group of process pid.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getpgid.001
 * MUTANT    getpgid.self
 * MUTANT    getpgid.pid
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
	report(setpgid(0, c1) == 0 ? 1 : 0, 0, 0);
}

/* c1 leads a group of its own, and c2 is a member of it (setup). */
static pid_t make_group(void)
{
	long v[3];
	pid_t c2;

	c1 = spawn(lead);
	get_report(v);
	if (v[0] != 1)
		pc_unresolved("setpgid(0, 0) failed in the child");
	c2 = spawn(join);
	get_report(v);
	if (v[0] != 1)
		pc_unresolved("setpgid(0, c1) failed in the second child");
	return c2;
}

int main(void)
{
	pid_t c2, a, b, me, own;

	pc_begin("XSH.getpgid.001");
	pipes_init();
	c2 = make_group();
	own = getpgrp();
	pc_test();
	a = getpgid(c1);
	pc_check(a == c1, "getpgid(c1) returned %ld (%s), expected %ld", (long)a, a == -1 ? pc_errname(errno) : "",
		 (long)c1);
	b = getpgid(c2);
	pc_check(b == c1, "getpgid(c2) returned %ld (%s), expected %ld", (long)b, b == -1 ? pc_errname(errno) : "",
		 (long)c1);
	me = getpgid(getpid());
	pc_check(me == own, "getpgid(getpid()) returned %ld, getpgrp() %ld", (long)me, (long)own);
	pc_pass();
}
