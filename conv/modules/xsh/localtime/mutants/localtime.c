/*
 * localtime.c - planted faults in localtime(), for the TZif checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "localtime.<fault>" and apply only
 * while PC_PHASE is "test".
 */
#define _GNU_SOURCE 1
#include <dlfcn.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct tm *(*fn_t)(const time_t *);

static const char *mutant(void)
{
	const char *ph = getenv("PC_PHASE");
	const char *m = getenv("PC_MUTANT");

	if (ph == NULL || strcmp(ph, "test") != 0 || m == NULL || strncmp(m, "localtime.", 10) != 0)
		return NULL;
	return m + 10;
}

struct tm *localtime(const time_t *t)
{
	static fn_t real;
	const char *m = mutant();
	time_t shifted;

	if (real == NULL)
		real = (fn_t)dlsym(RTLD_NEXT, "localtime");
	if (m != NULL && strcmp(m, "shifted") == 0) {
		shifted = *t + 3600;	/* an hour fast: as if the offset were wrong */
		return real(&shifted);
	}
	return real(t);
}
