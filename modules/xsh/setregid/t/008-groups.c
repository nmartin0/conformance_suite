/*
 * setregid() leaves the supplementary groups alone.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.setregid.008
 * VARIANT   getgroups.with_egid
 * VARIANT   getgroups.reversed
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

#define NG 256

/* The groups getgroups() reports, sorted, without dup and without skip (if not (gid_t)-1). */
static int group_set(gid_t *out, gid_t skip)
{
	gid_t gs[NG];
	int n = getgroups(NG, gs), i, j, k = 0;

	if (n < 0)
		return -1;
	for (i = 0; i < n; i++) {
		if (gs[i] == skip)
			continue;
		for (j = 0; j < k && out[j] != gs[i]; j++)
			;
		if (j == k)
			out[k++] = gs[i];
	}
	for (i = 1; i < k; i++)
		for (j = i; j > 0 && out[j - 1] > out[j]; j--) {
			gid_t t = out[j];
			out[j] = out[j - 1];
			out[j - 1] = t;
		}
	return k;
}

static int same_set(const gid_t *a, int na, const gid_t *b, int nb)
{
	int i;

	if (na != nb)
		return 0;
	for (i = 0; i < na; i++)
		if (a[i] != b[i])
			return 0;
	return 1;
}

/* Sets the supplementary groups to {65534} (setup); UNRESOLVED if not possible. */
static void set_groups(void)
{
	gid_t gs[NG];
	int n, i, found = 0;

	KNEED(pc_drop_groups() == 0, "cannot set the supplementary groups here (no setgroups())");
	n = getgroups(NG, gs);
	KNEED(n >= 1, "getgroups() reports no group after setgroups()");
	for (i = 0; i < n; i++)
		if (gs[i] == ID_A)
			found = 1;
	KNEED(found, "group 65534 is not reported after setgroups()");
}

static int try_setregid_c(void)
{
	return outcome(setregid((gid_t)-1, ID_C));
}

/* Real group ID 65534, effective and saved set-group-ID 65533, user IDs 65534 (setup). */
static void make_ab(void)
{
	kid_setup();
	KNEED(setregid(ID_A, ID_B) == 0, "setregid(65534, 65533) failed: %s", pc_errname(errno));
	KNEED(setuid(ID_A) == 0, "setuid(65534) failed: %s", pc_errname(errno));
	KNEED(getgid() == ID_A && getegid() == ID_B, "setregid() did not give 65534/65533");
	kid_test();
}

static void fn(void)
{
	gid_t before[NG], after[NG];
	int nb, na, r;

	kid_setup();
	set_groups();
	kid_test();
	nb = group_set(before, getegid());
	KNEED(nb >= 1, "getgroups() failed or reports no supplementary group");
	need_privileges(try_setregid_c, "setregid()");
	r = setregid(ID_B, ID_B);
	KCHECK(r == 0, "setregid(ID_B, ID_B) with privileges failed with %s", pc_errname(errno));
	na = group_set(after, getegid());
	KCHECK(na >= 0, "getgroups() failed after setregid(ID_B, ID_B): %s", pc_errname(errno));
	KCHECK(same_set(before, nb, after, na), "the supplementary groups changed after setregid(ID_B, ID_B)");
}

int main(void)
{
	pc_begin("XSH.setregid.008");
	if (getuid() != 0 || geteuid() != 0 || getgid() != 0 || getegid() != 0)
		pc_unresolved("needs a test process whose user and group IDs are all 0");
	pc_test();
	in_child(fn);
	pc_pass();
}
