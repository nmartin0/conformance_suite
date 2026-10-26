/*
 * pthread_mutexattr_settype.c - planted faults in
 * pthread_mutexattr_settype() and pthread_mutexattr_gettype(), for
 * mutation checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "pthread_mutexattr_settype.<fault>"
 * and apply while PC_PHASE is "test". The library records, in every
 * phase, which attributes objects have had a type set.
 */
#define _GNU_SOURCE 1
#include <dlfcn.h>
#include <errno.h>
#include <pthread.h>
#include <stdatomic.h>
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

/* Attributes objects whose type has been set. */
#define NREC 32
static const void *set_once[NREC];
static atomic_flag guard = ATOMIC_FLAG_INIT;

static int was_set(const void *a)
{
	int i, r = 0;

	while (atomic_flag_test_and_set(&guard))
		;
	for (i = 0; i < NREC; i++)
		if (set_once[i] == a)
			r = 1;
	atomic_flag_clear(&guard);
	return r;
}

static void mark_set(const void *a)
{
	int i;

	while (atomic_flag_test_and_set(&guard))
		;
	for (i = 0; i < NREC && set_once[i] != a; i++)
		if (set_once[i] == NULL) {
			set_once[i] = a;
			break;
		}
	atomic_flag_clear(&guard);
}

int pthread_mutexattr_settype(pthread_mutexattr_t *a, int type)
{
	static int (*real)(pthread_mutexattr_t *, int);
	const char *m = mutant_of("pthread_mutexattr_settype.");
	int valid = type == PTHREAD_MUTEX_NORMAL || type == PTHREAD_MUTEX_ERRORCHECK ||
	    type == PTHREAD_MUTEX_RECURSIVE || type == PTHREAD_MUTEX_DEFAULT;
	int r;

	if (real == NULL)
		real = (int (*)(pthread_mutexattr_t *, int))dlsym(RTLD_NEXT, "pthread_mutexattr_settype");
	if (IS("settype_ignored") && valid)
		return 0;
	if (IS("settype_first_only") && valid && was_set(a))
		return 0;
	if (IS("settype_no_check") && !valid)
		return 0;
	r = real(a, type);
	if (r == EINVAL && IS("settype_enotsup"))
		return ENOTSUP;
	if (r == 0)
		mark_set(a);
	return r;
}

int pthread_mutexattr_gettype(const pthread_mutexattr_t *a, int *type)
{
	static int (*real)(const pthread_mutexattr_t *, int *);
	const char *m = mutant_of("pthread_mutexattr_settype.");

	if (real == NULL)
		real = (int (*)(const pthread_mutexattr_t *, int *))dlsym(RTLD_NEXT, "pthread_mutexattr_gettype");
	if (IS("gettype_einval"))
		return EINVAL;
	if (IS("default_errorcheck") && !was_set(a)) {
		*type = PTHREAD_MUTEX_ERRORCHECK;
		return 0;
	}
	if (IS("gettype_constant")) {
		*type = PTHREAD_MUTEX_RECURSIVE;
		return 0;
	}
	return real(a, type);
}
