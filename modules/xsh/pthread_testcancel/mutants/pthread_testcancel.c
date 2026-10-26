/*
 * pthread_testcancel.c - planted faults in pthread_testcancel(), for
 * mutation checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "pthread_testcancel.<fault>" and
 * apply only while PC_PHASE is "test" (as set by pc.c in this process).
 * pthread_cancel() is wrapped to remember which threads have a request.
 */
#define _GNU_SOURCE 1
#include <dlfcn.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

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

static pthread_mutex_t req_lock = PTHREAD_MUTEX_INITIALIZER;
static pthread_t req[64];
static int nreq;

int pthread_cancel(pthread_t t)
{
	REAL(pthread_cancel);

	(void)pthread_mutex_lock(&req_lock);
	if (nreq < 64)
		req[nreq++] = t;
	(void)pthread_mutex_unlock(&req_lock);
	return real_pthread_cancel(t);
}

static int requested(void)
{
	int i, r = 0;

	(void)pthread_mutex_lock(&req_lock);
	for (i = 0; i < nreq; i++)
		if (pthread_equal(req[i], pthread_self()))
			r = 1;
	(void)pthread_mutex_unlock(&req_lock);
	return r;
}

void pthread_testcancel(void)
{
	const char *m = mutant_of("pthread_testcancel.");
	REAL(pthread_testcancel);

	if (IS("noop"))
		return;
	if (IS("always_acts") || (IS("ignores_disable") && requested()))
		pthread_exit(PTHREAD_CANCELED);	/* acts whatever the state, or with no request */
	real_pthread_testcancel();
}
