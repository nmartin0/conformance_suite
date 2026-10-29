/*
 * link.c - planted faults in link(), for the conventions checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "link.<fault>" and apply only while
 * PC_PHASE is "test".
 */
#define _GNU_SOURCE 1
#include <dlfcn.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef int (*fn_t)(const char *, const char *);

static const char *mutant(void)
{
	const char *ph = getenv("PC_PHASE");
	const char *m = getenv("PC_MUTANT");

	if (ph == NULL || strcmp(ph, "test") != 0 || m == NULL || strncmp(m, "link.", 5) != 0)
		return NULL;
	return m + 5;
}

int link(const char *a, const char *b)
{
	static fn_t real;
	const char *m = mutant();

	if (real == NULL)
		real = (fn_t)dlsym(RTLD_NEXT, "link");
	if (m != NULL && strcmp(m, "noop") == 0)
		return 0;	/* says it linked, did nothing: a file system without hard links */
	return real(a, b);
}
