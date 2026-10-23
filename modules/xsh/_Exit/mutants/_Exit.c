/*
 * _Exit.c - planted faults in _exit() and _Exit(), for mutation checks
 * (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "_Exit.<fault>" and apply only
 * while PC_PHASE is "test".
 */
#define _GNU_SOURCE 1
#include <dlfcn.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef void (*exit_fn)(int);

static const char *mutant(void)
{
	const char *ph = getenv("PC_PHASE");
	const char *m = getenv("PC_MUTANT");
	if (ph == NULL || strcmp(ph, "test") != 0 || m == NULL ||
	    strncmp(m, "_Exit.", 6) != 0)
		return NULL;
	return m + 6;
}

#define IS(name) (m != NULL && strcmp(m, name) == 0)

_Noreturn static void faulty(exit_fn real, int status)
{
	const char *m = mutant();

	if (IS("calls_atexit"))
		exit(status);		/* runs atexit() handlers and flushes */
	if (IS("flushes"))
		(void)fflush(NULL);
	if (IS("status_wrong"))
		status ^= 1;
	if (IS("unblocks")) {
		sigset_t none;
		sigemptyset(&none);
		(void)sigprocmask(SIG_SETMASK, &none, NULL);	/* pending signals arrive */
	}
	if (IS("raises"))
		(void)raise(SIGTERM);
	if (IS("calls_quick"))
		quick_exit(status);	/* runs at_quick_exit() handlers */
	real(status);
	for (;;)
		;
}

void _exit(int status)
{
	static exit_fn real;

	if (real == NULL)
		real = (exit_fn)dlsym(RTLD_NEXT, "_exit");
	faulty(real, status);
}

void _Exit(int status)
{
	static exit_fn real;

	if (real == NULL)
		real = (exit_fn)dlsym(RTLD_NEXT, "_Exit");
	faulty(real, status);
}
