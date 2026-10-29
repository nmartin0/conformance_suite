/*
 * mkdir.c - planted faults in mkdir(), for the conventions checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "mkdir.<fault>" and apply only while
 * PC_PHASE is "test".
 */
#define _GNU_SOURCE 1
#include <dlfcn.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

typedef int (*fn_t)(const char *, mode_t);

static const char *mutant(void)
{
	const char *ph = getenv("PC_PHASE");
	const char *m = getenv("PC_MUTANT");

	if (ph == NULL || strcmp(ph, "test") != 0 || m == NULL || strncmp(m, "mkdir.", 6) != 0)
		return NULL;
	return m + 6;
}

int mkdir(const char *path, mode_t mode)
{
	static fn_t real;
	const char *m = mutant();
	int r;

	if (real == NULL)
		real = (fn_t)dlsym(RTLD_NEXT, "mkdir");
	r = real(path, mode);
	if (r == 0 && m != NULL && strcmp(m, "loose") == 0)
		(void)chmod(path, 0755);	/* ignores the requested mode */
	return r;
}
