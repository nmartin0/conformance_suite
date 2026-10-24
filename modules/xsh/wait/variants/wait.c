/*
 * wait.c - conforming variants of wait() and waitpid(), for the variant
 * checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; see core/pcvariant.h. Each variant says why the
 * standard allows it.
 */
#define _GNU_SOURCE 1
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>
#include "pcvariant.h"

/*
 * "status_encoding": the status value uses a layout of its own (the header
 * half is wait.status_encoding.h, which gives the macros that read it).
 * XSH wait() fixes only what the W* macros report and that the value is 0
 * exactly for a clean exit (or the end of the last thread); the RATIONALE
 * there says the macros exist so that no bit layout is imposed. Here:
 *   exited with code c       c (0 to 255; so a clean exit is 0)
 *   killed by signal n       0x1000000 | n << 8, plus 0x100000 when a core
 *                            image was attempted
 *   stopped by signal n      0x2000000 | n << 8
 *   continued                0x3000000
 * This system attempts a core image for every signal whose default action
 * is abnormal termination with additional actions (XBD <signal.h> "A"),
 * whatever RLIMIT_CORE says: the WCOREDUMP RATIONALE in wait() allows a
 * non-zero WCOREDUMP when the limit prevented the file. system() and
 * pclose() return the same layout, as they must on such a system.
 */
static int core_default(int sig)
{
	switch (sig) {
	case SIGABRT: case SIGBUS: case SIGFPE: case SIGILL: case SIGQUIT:
	case SIGSEGV: case SIGSYS: case SIGTRAP: case SIGXCPU: case SIGXFSZ:
		return 1;
	}
	return 0;
}

static int encode(int s)
{
	if (WIFEXITED(s))
		return WEXITSTATUS(s);
	if (WIFSIGNALED(s))
		return 0x1000000 | (WTERMSIG(s) & 0xff) << 8 |
		    (WCOREDUMP(s) || core_default(WTERMSIG(s)) ? 0x100000 : 0);
	if (WIFSTOPPED(s))
		return 0x2000000 | (WSTOPSIG(s) & 0xff) << 8;
#ifdef WIFCONTINUED
	if (WIFCONTINUED(s))
		return 0x3000000;
#endif
	return s;
}

/*
 * "ext_option": option bit 0x400000 is an implementation extension flag
 * that has no effect on these calls. The options argument is built from
 * the flags <sys/wait.h> defines, and an implementation may define more
 * (XSH 2.2.2 reserves the W prefix in <sys/wait.h> for it); only a value
 * that is not valid on this system must fail with EINVAL. Every other
 * undefined bit still fails with EINVAL, as the real system does.
 */
#define EXT_BIT 0x400000

pid_t waitpid(pid_t pid, int *st, int opt)
{
	const char *v = variant_of("wait.");
	int s = 0;
	pid_t r;
	REAL(waitpid);

	if (IS("ext_option"))
		opt &= ~EXT_BIT;
	if (!IS("status_encoding"))
		return real_waitpid(pid, st, opt);
	r = real_waitpid(pid, &s, opt);
	if (r > 0 && st != NULL)
		*st = encode(s);
	return r;
}

pid_t wait(int *st)
{
	const char *v = variant_of("wait.");
	int s = 0;
	pid_t r;
	REAL(wait);
	REAL(waitpid);

	if (!IS("status_encoding"))
		return real_wait(st);
	r = real_waitpid((pid_t)-1, &s, 0);
	if (r > 0 && st != NULL)
		*st = encode(s);
	return r;
}

int system(const char *cmd)
{
	const char *v = variant_of("wait.");
	int r;
	REAL(system);

	r = real_system(cmd);
	if (IS("status_encoding") && cmd != NULL && r != -1)
		r = encode(r);
	return r;
}

int pclose(FILE *f)
{
	const char *v = variant_of("wait.");
	int r;
	REAL(pclose);

	r = real_pclose(f);
	if (IS("status_encoding") && r != -1)
		r = encode(r);
	return r;
}

/*
 * "slow_stop": a process that stops itself with raise(SIGSTOP) is slow to
 * get there (half a second), as on a loaded system. Nothing in the
 * standard bounds how soon a process is scheduled, so a parent can never
 * assume that a child has stopped after a fixed delay.
 */
int raise(int sig)
{
	const char *v = variant_of("wait.");
	REAL(raise);

	if (IS("slow_stop") && sig == SIGSTOP) {
		struct timespec ts = { 0, 500000000 };
		while (nanosleep(&ts, &ts) == -1 && errno == EINTR)
			;
	}
	return real_raise(sig);
}
