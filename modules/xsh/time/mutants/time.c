/*
 * time.c - planted faults in time(), for mutation checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "time.<fault>" and apply only while
 * PC_PHASE is "test" (as set by pc.c in this process or a parent).
 */
#define _GNU_SOURCE 1
#include <dlfcn.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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

time_t time(time_t *tloc)
{
	REAL(time);
	const char *m = mutant_of("time.");
	static time_t first;
	time_t v;

	if (m == NULL)
		return real_time(tloc);
	if (IS("minus_one")) {
		errno = EOVERFLOW;
		return (time_t)-1;
	}
	if (IS("null_fails") && tloc == NULL) {
		errno = EFAULT;
		return (time_t)-1;
	}
	v = real_time(NULL);
	if (IS("frozen")) {
		/* the value of the first call, for ever */
		if (first == 0)
			first = v;
		v = first;
	}
	if (IS("offset"))
		v += 3600;	/* local time of a zone one hour east, not seconds since the Epoch */
	if (tloc != NULL && !IS("tloc_untouched"))
		*tloc = IS("tloc_other") ? v - 1 : v;
	return v;
}
