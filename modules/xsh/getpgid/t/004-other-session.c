/*
 * getpgid() of a process in another session: its group, or EPERM.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getpgid.004
 * MUTANT    getpgid.self
 * MUTANT    getpgid.hide_esrch
 * VARIANT   getpgid.deny_other_session
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

static void leader(void)
{
	report(result((long)setsid()), 0, 0);
}

int main(void)
{
	pid_t s, r;
	long v[3];
	int e;

	pc_begin("XSH.getpgid.004");
	pipes_init();
	s = spawn(leader);
	get_report(v);
	if (v[0] != (long)s)
		pc_unresolved("setsid() in the child returned %ld (%s)", v[0], ename(v[0]));
	pc_test();
	r = getpgid(s);
	e = errno;
	if (r == -1) {
		pc_check(e == EPERM, "getpgid(<process in another session>) failed with %s, expected EPERM or success",
			 pc_errname(e));
		pc_note("the implementation does not let the caller see that group (EPERM)");
	} else {
		pc_check(r == s, "getpgid(<leader of another session>) returned %ld, expected %ld", (long)r, (long)s);
	}
	pc_pass();
}
