/*
 * ferror.c - planted faults in ferror(), for mutation checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "<function>.<fault>" and apply only
 * while PC_PHASE is "test" (as set by pc.c in this process or a parent).
 */
#define _GNU_SOURCE 1
#include <dlfcn.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdio_ext.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <wchar.h>

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



int ferror(FILE *f)
{
	REAL(ferror);
	const char *m = mutant_of("ferror.");

	if (m == NULL)
		return real_ferror(f);
	if (IS("or_eof"))
		{
		REAL(feof);

		return real_ferror(f) || real_feof(f);
	}	/* tests the wrong mask */
	if (IS("ignores"))
		return 0;	/* a stub that never reports an error */
	if (IS("clears_errno")) {
		int r = real_ferror(f);

		errno = 0;
		return r;
	}
	if (IS("sets_errno")) {
		int r = real_ferror(f);

		if (r)
			errno = EIO;	/* reports the condition through errno too */
		return r;
	}
	return real_ferror(f);
}
