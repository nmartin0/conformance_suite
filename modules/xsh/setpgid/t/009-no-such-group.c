/*
 * setpgid() to a group that is not in the caller's session fails with EPERM.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.setpgid.009
 * MUTANT    setpgid.nogroup_ok
 * MUTANT    setpgid.eperm_esrch
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
 * The work runs in a child, which reports its verdict to the test process
 * over a pipe (K_PASS, K_FAIL or K_UNRES, with a message).
 */
enum { K_PASS, K_FAIL, K_UNRES };
static int kid_fd = -1;

_Noreturn static void kid_end(int code, const char *fmt, ...)
{
	char msg[400];
	va_list ap;
	ssize_t w;

	va_start(ap, fmt);
	(void)vsnprintf(msg, sizeof msg, fmt, ap);
	va_end(ap);
	w = write(kid_fd, msg, strlen(msg));
	(void)w;
	_exit(code);
}

#define KCHECK(cond, ...) do { if (!(cond)) kid_end(K_FAIL, __VA_ARGS__); } while (0)
#define KNEED(cond, ...) do { if (!(cond)) kid_end(K_UNRES, __VA_ARGS__); } while (0)

/*
 * Planted faults (CI) act only in the test phase: a child's own setup
 * steps run outside it.
 */
static void kid_setup(void)
{
	(void)setenv("PC_PHASE", "setup", 1);
}

static void kid_test(void)
{
	(void)setenv("PC_PHASE", "test", 1);
}

/* Runs fn in a child (after pc_test()); the child's verdict is the test's. */
static void in_child(void (*fn)(void))
{
	char msg[401];
	size_t got = 0;
	ssize_t n;
	int p[2], st;
	pid_t kid;

	if (pipe(p) != 0)
		pc_unresolved("pipe failed: %s", pc_errname(errno));
	kid = fork();
	if (kid == -1)
		pc_unresolved("fork failed: %s", pc_errname(errno));
	if (kid == 0) {
		(void)close(p[0]);
		kid_fd = p[1];
		fn();
		kid_end(K_PASS, "");
	}
	(void)close(p[1]);
	for (;;) {
		n = read(p[0], msg + got, sizeof msg - 1 - got);
		if (n > 0) {
			got += (size_t)n;
			if (got == sizeof msg - 1)
				break;
		} else if (n == 0 || errno != EINTR) {
			break;
		}
	}
	msg[got] = '\0';
	(void)close(p[0]);
	while (waitpid(kid, &st, 0) == -1)
		if (errno != EINTR)
			pc_unresolved("waitpid failed: %s", pc_errname(errno));
	if (!WIFEXITED(st))
		pc_fail("the child ended abnormally (status %#x)", (unsigned)st);
	if (WEXITSTATUS(st) == K_FAIL)
		pc_fail("%s", msg);
	if (WEXITSTATUS(st) == K_UNRES)
		pc_unresolved("%s", msg);
	if (WEXITSTATUS(st) != K_PASS)
		pc_unresolved("the child exited with status %d", WEXITSTATUS(st));
}

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

static pid_t gone, other;

static void leader(void)
{
	report(result((long)setsid()), 0, 0);
}

static void fn(void)
{
	pid_t g = getpgrp();
	int r, e;

	r = setpgid(0, gone);
	e = errno;
	KNEED(r == -1 || still_gone(gone), "process ID %ld was reused during the test", (long)gone);
	KCHECK(r == -1, "setpgid(0, <no such group>) returned %d, expected -1", r);
	KCHECK(e == EPERM, "setpgid(0, <no such group>) failed with %s, expected EPERM", pc_errname(e));
	r = setpgid(0, other);
	e = errno;
	KCHECK(r == -1, "setpgid(0, <group in another session>) returned %d, expected -1", r);
	KCHECK(e == EPERM, "setpgid(0, <group in another session>) failed with %s, expected EPERM", pc_errname(e));
	KCHECK(getpgrp() == g, "the caller's group changed after failed calls");
}

int main(void)
{
	long v[3];

	pc_begin("XSH.setpgid.009");
	pipes_init();
	other = spawn(leader);
	get_report(v);
	if (v[0] != (long)other)
		pc_unresolved("setsid() in the helper returned %ld (%s)", v[0], ename(v[0]));
	gone = gone_pid();
	pc_test();
	in_child(fn);
	pc_pass();
}
