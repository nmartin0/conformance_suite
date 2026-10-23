/*
 * umask.c - planted faults in umask(), for mutation checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "<function>.<fault>" and apply only
 * while PC_PHASE is "test" (as set by pc.c in this process or a parent).
 */
#define _GNU_SOURCE 1
#include <dirent.h>
#include <dlfcn.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

static char phase[16];

/* pc.c sets PC_PHASE with setenv(); remember it in this process. */
int setenv(const char *name, const char *value, int overwrite)
{
	static int (*real)(const char *, const char *, int);

	if (real == NULL)
		real = (int (*)(const char *, const char *, int))dlsym(RTLD_NEXT, "setenv");
	if (strcmp(name, "PC_PHASE") == 0 && strlen(value) < sizeof phase)
		strcpy(phase, value);
	return real(name, value, overwrite);
}

static const char *mutant_of(const char *prefix)
{
	const char *m = getenv("PC_MUTANT");
	size_t n = strlen(prefix);

	if (strcmp(phase, "test") != 0 || m == NULL || strncmp(m, prefix, n) != 0)
		return NULL;
	return m + n;
}

#define IS(name) (m != NULL && strcmp(m, name) == 0)
#define REAL(fn) static __typeof__(fn) *real_##fn; \
	if (real_##fn == NULL) real_##fn = (__typeof__(fn) *)dlsym(RTLD_NEXT, #fn)

#include <sys/resource.h>
#include <time.h>
static int fail(int e)
{
	errno = e;
	return -1;
}

mode_t umask(mode_t cmask)
{
	REAL(umask);
	const char *m = mutant_of("umask.");
	mode_t old;

	if (m == NULL)
		return real_umask(cmask);
	if (IS("noop")) {
		old = real_umask(0);
		(void)real_umask(old);
		return old;
	}
	if (IS("keeps_svtx")) {
		static mode_t extra;
		old = real_umask(cmask);
		old |= extra;
		extra = cmask & S_ISVTX;
		return old;
	}
	old = real_umask(cmask);
	if (IS("returns_new"))
		return cmask;
	return old;
}
