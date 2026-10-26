/*
 * geteuid() returns the effective user ID.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.geteuid.001
 * MUTANT    geteuid.real
 * MUTANT    geteuid.cached
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

static void fn(void)
{
	unsigned long u = (unsigned long)geteuid();

	KCHECK(u == 0, "geteuid() returned %lu in a process whose user IDs are 0", u);
	kid_setup();
	KNEED(seteuid(ID_A) == 0, "seteuid(65534) failed: %s", pc_errname(errno));
	kid_test();
	u = (unsigned long)geteuid();
	KCHECK(u == ID_A, "after seteuid(65534), geteuid() returned %lu, expected 65534", u);
	kid_setup();
	KNEED(seteuid(0) == 0, "seteuid(0) failed: %s", pc_errname(errno));
	kid_test();
	u = (unsigned long)geteuid();
	KCHECK(u == 0, "after seteuid(0), geteuid() returned %lu, expected 0", u);
}

int main(void)
{
	pc_begin("XSH.geteuid.001");
	if (getuid() != 0 || geteuid() != 0 || getgid() != 0 || getegid() != 0)
		pc_unresolved("needs a test process whose user and group IDs are all 0");
	pc_test();
	in_child(fn);
	pc_pass();
}
