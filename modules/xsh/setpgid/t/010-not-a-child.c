/*
 * setpgid() of a process that is not the caller or its child fails with ESRCH.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.setpgid.010
 * MUTANT    setpgid.esrch_eperm
 * MUTANT    setpgid.esrch_ok
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

/* The ID of a reaped child, used by no process and no process group now. */
static pid_t gone_pid(void)
{
	pid_t pid = fork();
	int st;

	if (pid == -1)
		pc_unresolved("fork failed: %s", pc_errname(errno));
	if (pid == 0)
		_exit(0);
	while (waitpid(pid, &st, 0) == -1)
		if (errno != EINTR)
			pc_unresolved("waitpid failed: %s", pc_errname(errno));
	if (kill(pid, 0) == 0 || errno != ESRCH || kill(-pid, 0) == 0 || errno != ESRCH)
		pc_unresolved("process ID %ld is in use again", (long)pid);
	return pid;
}

static int still_gone(pid_t pid)
{
	return kill(pid, 0) == -1 && errno == ESRCH && kill(-pid, 0) == -1 && errno == ESRCH;
}

static void idle(void)
{
}

/* The child: create a grandchild and report its ID. */
static void parent_of(void)
{
	pid_t g = spawn(idle);

	report((long)g, 0, 0);
}

int main(void)
{
	pid_t g, gone;
	long v[3];
	int r;

	pc_begin("XSH.setpgid.010");
	pipes_init();
	(void)spawn(parent_of);
	get_report(v);
	g = (pid_t)v[0];
	gone = gone_pid();
	pc_test();
	pc_expect_err(setpgid(g, g), ESRCH, "setpgid(<grandchild>, <its ID>)");
	r = setpgid(gone, gone);
	if (r != -1 && !still_gone(gone))
		pc_unresolved("process ID %ld was reused during the test", (long)gone);
	pc_expect_err(r, ESRCH, "setpgid(<reaped child>, <its ID>)");
	pc_pass();
}
