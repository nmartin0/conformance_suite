/*
 * getpid.c - planted faults in getpid(), for mutation checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "getpid.<fault>" and apply only while
 * PC_PHASE is "test".
 */
#define _GNU_SOURCE 1
#include <dlfcn.h>
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static const char *mutant(void)
{
	const char *ph = getenv("PC_PHASE");
	const char *m = getenv("PC_MUTANT");
	size_t n = strlen("getpid.");

	if (ph == NULL || strcmp(ph, "test") != 0 || m == NULL || strncmp(m, "getpid.", n) != 0)
		return NULL;
	return m + n;
}

#define IS(name) (m != NULL && strcmp(m, name) == 0)
#define REAL(fn) static __typeof__(fn) *real_##fn; \
	if (real_##fn == NULL) real_##fn = (__typeof__(fn) *)dlsym(RTLD_NEXT, #fn)

static pid_t cache;

pid_t getpid(void)
{
	REAL(getpid);
	const char *m = mutant();

	if (IS("cached")) {	/* a cache that fork() does not refresh */
		if (cache == 0)
			cache = real_getpid();
		return cache;
	}
	if (IS("parent"))
		return getppid();
	return real_getpid();
}
