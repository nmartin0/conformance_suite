/*
 * getsid(pid) returns the session leader's group ID.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getsid.001
 * MUTANT    getsid.pgrp
 * MUTANT    getsid.self
 * VARIANT   getsid.deny_other_session
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

static int go[2];

/* c3: stays in the session leader's group. */
static void stay(void)
{
}

/*
 * c2: leads a group of its own in c1's session; reports when ready, waits
 * for the word, then makes the calls under test (in the test phase).
 */
static pid_t c1_id, c3_id;

static void member(void)
{
	char c;
	int r = setpgid(0, 0);

	report(r == 0 ? 1 : 0, (long)getpid(), 0);
	while (read(go[0], &c, 1) == -1 && errno == EINTR)
		;
	(void)setenv("PC_PHASE", "test", 1);
	report(result((long)getsid(c1_id)), result((long)getsid(getpid())), result((long)getsid(c3_id)));
}

/* c1: starts a session and creates c3 and then c2 in it. */
static void session(void)
{
	pid_t s = setsid();

	c1_id = getpid();
	report(result((long)s), 0, 0);
	if (s == -1)
		return;
	c3_id = spawn(stay);
	(void)spawn(member);
}

static pid_t c1, c2;

/* Builds the session (setup); c2 then waits for the word on go[]. */
static void make_session(void)
{
	long v[3];

	if (pipe(go) != 0)
		pc_unresolved("pipe failed");
	c1 = spawn(session);
	get_report(v);
	if (v[0] != (long)c1)
		pc_unresolved("setsid() in the child returned %ld (%s)", v[0], ename(v[0]));
	get_report(v);
	if (v[0] != 1)
		pc_unresolved("setpgid(0, 0) failed in the session's second member");
	c2 = (pid_t)v[1];
}

static void word(void)
{
	ssize_t w = write(go[1], "x", 1);

	if (w != 1)
		pc_unresolved("cannot wake the session member");
}

int main(void)
{
	long v[3];
	pid_t r;
	int e;

	pc_begin("XSH.getsid.001");
	pipes_init();
	make_session();
	pc_test();
	word();
	get_report(v);
	pc_check(v[0] == (long)c1, "getsid(c1) in c2 returned %ld (%s), expected %ld", v[0], ename(v[0]), (long)c1);
	pc_check(v[1] == (long)c1, "getsid(c2) in c2 returned %ld (%s), expected %ld", v[1], ename(v[1]), (long)c1);
	pc_check(v[2] == (long)c1, "getsid(c3) in c2 returned %ld (%s), expected %ld", v[2], ename(v[2]), (long)c1);
	r = getsid(c2);
	e = errno;
	if (r == -1) {
		pc_check(e == EPERM, "getsid(<process in another session>) failed with %s, expected EPERM or success",
			 pc_errname(e));
		pc_note("the implementation does not let the caller see that session (EPERM)");
	} else {
		pc_check(r == c1, "getsid(c2) from another session returned %ld, expected %ld", (long)r, (long)c1);
	}
	pc_pass();
}
