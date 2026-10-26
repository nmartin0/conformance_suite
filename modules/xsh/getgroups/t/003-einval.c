/*
 * getgroups() with a size smaller than the count fails with EINVAL.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getgroups.003
 * MUTANT    getgroups.neg_ok
 * MUTANT    getgroups.neg_efault
 * VARIANT   getgroups.with_egid
 * VARIANT   getgroups.reversed
 * VARIANT   getgroups.scribbles
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

/* IDs given to the process, one it never holds (probes), and a refused target. */
#define ID_A 65534
#define ID_B 65533
#define ID_C 65532
#define ID_D 65531

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

static void fn(void)
{
	gid_t list[NG];
	volatile int neg = -1;	/* not a constant: some compilers warn about the call */
	int n, r, e;

	kid_setup();
	set_groups();
	kid_test();
	r = getgroups(neg, list);
	e = errno;
	KCHECK(r == -1, "getgroups(-1, list) returned %d, expected -1", r);
	KCHECK(e == EINVAL, "getgroups(-1, list) failed with %s, expected EINVAL", pc_errname(e));
	n = getgroups(NG, list);
	KNEED(n >= 1, "getgroups(%d) failed: %s", NG, pc_errname(errno));
	if (n < 2) {
		pc_note("only one group to report: a size of count - 1 would be 0");
		return;
	}
	r = getgroups(n - 1, list);
	e = errno;
	KCHECK(r == -1, "getgroups(%d, list) with %d groups returned %d, expected -1", n - 1, n, r);
	KCHECK(e == EINVAL, "getgroups(%d, list) with %d groups failed with %s, expected EINVAL", n - 1, n,
	       pc_errname(e));
}

int main(void)
{
	pc_begin("XSH.getgroups.003");
	if (getuid() != 0 || geteuid() != 0 || getgid() != 0 || getegid() != 0)
		pc_unresolved("needs a test process whose user and group IDs are all 0");
	pc_test();
	in_child(fn);
	pc_pass();
}
