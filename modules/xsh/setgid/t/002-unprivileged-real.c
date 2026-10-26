/*
 * setgid() to the real group ID without privileges sets the effective ID.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.setgid.002
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

/* Outcome of a call: it worked, it failed with EPERM, or anything else. */
enum { P_OK, P_EPERM, P_OTHER, P_STEP };

static int outcome(int r)
{
	return r == 0 ? P_OK : errno == EPERM ? P_EPERM : P_OTHER;
}

/*
 * Runs fn in a disposable grandchild, in the setup phase (no planted
 * fault), and returns what fn returns (P_OK, P_EPERM, P_OTHER, or P_STEP
 * when a preparatory step failed); P_STEP if the grandchild cannot run.
 */
static int probe(int (*fn)(void))
{
	pid_t g;
	int st;

	g = fork();
	if (g == -1)
		return P_STEP;
	if (g == 0) {
		kid_setup();
		_exit(fn());
	}
	while (waitpid(g, &st, 0) == -1)
		if (errno != EINTR)
			return P_STEP;
	return WIFEXITED(st) && WEXITSTATUS(st) <= P_STEP ? WEXITSTATUS(st) : P_STEP;
}

/*
 * Appropriate privileges are implementation-defined (XBD 3.21): the only
 * evidence is whether a call that needs them works. fn makes such a call.
 */
static void need_privileges(int (*fn)(void), const char *what)
{
	KNEED(probe(fn) == P_OK, "the process has no appropriate privileges for %s here", what);
}

static void need_no_privileges(int (*fn)(void), const char *what)
{
	int r = probe(fn);

	KNEED(r != P_OK, "the process still has appropriate privileges for %s", what);
	KNEED(r == P_EPERM, "the privilege probe (%s) failed with another error", what);
}

/* IDs given to the process, one it never holds (probes), and a refused target. */
#define ID_A 65534
#define ID_B 65533
#define ID_C 65532
#define ID_D 65531

static void expect_gids(unsigned long r, unsigned long e, const char *when)
{
	unsigned long gr = (unsigned long)getgid(), ge = (unsigned long)getegid();

	KCHECK(gr == r && ge == e, "%s: real/effective group IDs are %lu/%lu, expected %lu/%lu",
	       when, gr, ge, r, e);
}

static gid_t probe_g1, probe_g2;
static uid_t probe_drop = (uid_t)-1;

static int try_setegid_c(void)
{
	if (probe_drop != (uid_t)-1 && setuid(probe_drop) != 0)
		return P_STEP;
	return outcome(setegid(ID_C));
}

/*
 * After setuid(probe_drop) when that is not (uid_t)-1 (giving up the
 * privileges of user ID 0), setegid(probe_g1), then setegid(probe_g2) if
 * that is not (gid_t)-1.
 */
static int try_setegid(void)
{
	if (probe_drop != (uid_t)-1 && setuid(probe_drop) != 0)
		return P_STEP;
	if (probe_g2 == (gid_t)-1)
		return outcome(setegid(probe_g1));
	if (setegid(probe_g1) != 0)
		return P_STEP;
	return outcome(setegid(probe_g2));
}

/* As try_setegid(), with no privilege for setegid() left (else UNRESOLVED). */
static int setegid_works(uid_t drop, gid_t g1, gid_t g2)
{
	probe_drop = drop;
	need_no_privileges(try_setegid_c, "setegid()");
	probe_g1 = g1;
	probe_g2 = g2;
	return probe(try_setegid);
}

static int try_setgid_c(void)
{
	return outcome(setgid(ID_C));
}

static void fn(void)
{
	int r;

	kid_setup();
	KNEED(setegid(ID_B) == 0 && seteuid(ID_A) == 0, "setegid(65533) or seteuid(65534) failed: %s",
	      pc_errname(errno));
	kid_test();
	need_no_privileges(try_setgid_c, "setgid()");
	r = setgid(0);
	KCHECK(r == 0, "setgid(<real group ID>) without privileges failed with %s", pc_errname(errno));
	expect_gids(0, 0, "after setgid(0)");
}

int main(void)
{
	pc_begin("XSH.setgid.002");
	if (getuid() != 0 || geteuid() != 0 || getgid() != 0 || getegid() != 0)
		pc_unresolved("needs a test process whose user and group IDs are all 0");
	pc_test();
	in_child(fn);
	pc_pass();
}
