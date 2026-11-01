/*
 * exit.c - planted faults in exit(), for mutation checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "exit.<fault>" and apply only while
 * PC_PHASE is "test". Only faults that need no access to the handlers
 * registered with atexit() are planted here: the registration functions
 * may be linked into the program and bound before any preload.
 */
#define _GNU_SOURCE 1
#include <dlfcn.h>
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
	    strncmp(m, "exit.", 5) != 0)
		return NULL;
	return m + 5;
}

#define IS(name) (m != NULL && strcmp(m, name) == 0)

void exit(int status)
{
	static exit_fn real;
	const char *m = mutant();

	if (real == NULL)
		real = (exit_fn)dlsym(RTLD_NEXT, "exit");
	if (m == NULL)
		real(status);
	if (IS("returns"))
		return;
	if (IS("status_wrong"))
		status ^= 1;
	if (IS("no_atexit"))
		_exit(status);
	real(status);
	_exit(status);	/* not reached */
}
