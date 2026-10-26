/*
 * pthread_equal.c - planted faults in pthread_equal(), for mutation
 * checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "pthread_equal.<fault>" and apply
 * only while PC_PHASE is "test". The PTHREAD_NULL faults take an all-zero
 * pthread_t as PTHREAD_NULL, as the systems the suite is tried on (and
 * the variant pthread_equal.null_zero) define it.
 */
#define _GNU_SOURCE 1
#include <dlfcn.h>
#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
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

static void __attribute__((unused)) nap_ms(long ms)
{
	struct timespec ts = { ms / 1000, (ms % 1000) * 1000000L };

	while (nanosleep(&ts, &ts) == -1 && errno == EINTR)
		;
}

#define PREFIX "pthread_equal."

/* Some C libraries also define pthread_equal() as a macro; this is the function. */
#undef pthread_equal

static int is_null(pthread_t t)
{
	pthread_t z;

	memset(&z, 0, sizeof z);
	return memcmp(&t, &z, sizeof t) == 0;
}

int pthread_equal(pthread_t a, pthread_t b)
{
	REAL(pthread_equal);
	const char *m = mutant_of(PREFIX);
	int r = real_pthread_equal(a, b);

	if (IS("always_zero"))
		return 0;
	if (IS("always_one"))
		return 1;
	if (IS("inverted"))
		return !r;
	if (IS("null_matches") && (is_null(a) || is_null(b)))
		return 1;	/* PTHREAD_NULL taken as a wildcard */
	if (IS("null_unequal") && is_null(a) && is_null(b))
		return 0;
	return r;
}
