/*
 * geteuid.c - planted faults in geteuid(), for mutation checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "geteuid.<fault>" and apply only while
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
	size_t n = strlen("geteuid.");

	if (ph == NULL || strcmp(ph, "test") != 0 || m == NULL || strncmp(m, "geteuid.", n) != 0)
		return NULL;
	return m + n;
}

#define IS(name) (m != NULL && strcmp(m, name) == 0)
#define REAL(fn) static __typeof__(fn) *real_##fn; \
	if (real_##fn == NULL) real_##fn = (__typeof__(fn) *)dlsym(RTLD_NEXT, #fn)

static uid_t cache;
static int cached;

uid_t geteuid(void)
{
	REAL(geteuid);
	const char *m = mutant();
	uid_t u;

	if (IS("real"))
		return getuid();
	if (IS("cached")) {	/* a cache that ID changes do not refresh */
		if (!cached) {
			cache = real_geteuid();
			cached = 1;
		}
		return cache;
	}
	u = real_geteuid();
	if (IS("clobbers_errno"))
		errno = 0;
	if (IS("sets_errno"))
		errno = ENOSYS;
	return u;
}
