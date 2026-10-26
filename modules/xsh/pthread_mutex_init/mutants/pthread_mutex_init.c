/*
 * pthread_mutex_init.c - planted faults in pthread_mutex_init() and
 * pthread_mutex_destroy() (and in how the lock functions treat the
 * objects they make), for mutation checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "pthread_mutex_init.<fault>" and
 * apply while PC_PHASE is "test". The library records, in every phase,
 * the addresses passed to pthread_mutex_init() and those destroyed.
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

/* Addresses and their state: 1 initialized, 2 destroyed. */
#define NREC 64
static void *addrs[NREC];
static int states[NREC];
static atomic_flag guard = ATOMIC_FLAG_INIT;

static void take(void) { while (atomic_flag_test_and_set(&guard)) ; }
static void give(void) { atomic_flag_clear(&guard); }

static int state_of(void *p)
{
	int i, s = 0;

	take();
	for (i = 0; i < NREC; i++)
		if (addrs[i] == p)
			s = states[i];
	give();
	return s;
}

static void set_state(void *p, int s)
{
	int i, slot = -1;

	take();
	for (i = 0; i < NREC && slot < 0; i++)
		if (addrs[i] == p)
			slot = i;
	for (i = 0; i < NREC && slot < 0; i++)
		if (addrs[i] == NULL)
			slot = i;
	if (slot < 0)
		slot = 0;
	addrs[slot] = p;
	states[slot] = s;
	give();
}

typedef int (*mfn)(pthread_mutex_t *);

int pthread_mutex_init(pthread_mutex_t *mx, const pthread_mutexattr_t *attr)
{
	static int (*real)(pthread_mutex_t *, const pthread_mutexattr_t *);
	static mfn real_lock;
	const char *m = mutant_of("pthread_mutex_init.");
	int r;

	if (real == NULL) {
		real = (int (*)(pthread_mutex_t *, const pthread_mutexattr_t *))dlsym(RTLD_NEXT, "pthread_mutex_init");
		real_lock = (mfn)dlsym(RTLD_NEXT, "pthread_mutex_lock");
	}
	if (IS("init_null_einval") && attr == NULL)
		return EINVAL;
	if (IS("reinit_ebusy") && state_of(mx) == 2)
		return EBUSY;
	r = real(mx, IS("init_ignores_attr") ? NULL : attr);
	if (r != 0)
		return r;
	set_state(mx, 1);
	if (IS("init_locked"))
		(void)real_lock(mx);
	return 0;
}

int pthread_mutex_destroy(pthread_mutex_t *mx)
{
	static mfn real;
	const char *m = mutant_of("pthread_mutex_init.");
	int r;

	if (real == NULL)
		real = (mfn)dlsym(RTLD_NEXT, "pthread_mutex_destroy");
	if (IS("destroy_ebusy"))
		return EBUSY;
	r = real(mx);
	if (r == 0)
		set_state(mx, 2);
	return r;
}

/*
 * "static_needs_init": the lock functions accept only mutexes passed to
 * pthread_mutex_init() (no PTHREAD_MUTEX_INITIALIZER support).
 * "pshared_by_address": they accept a mutex only at the address it was
 * initialized at (no use through another mapping).
 */
static int refused(const char *m, pthread_mutex_t *mx)
{
	return (IS("static_needs_init") || IS("pshared_by_address")) && state_of(mx) != 1;
}

int pthread_mutex_lock(pthread_mutex_t *mx)
{
	static mfn real;
	const char *m = mutant_of("pthread_mutex_init.");

	if (real == NULL)
		real = (mfn)dlsym(RTLD_NEXT, "pthread_mutex_lock");
	return refused(m, mx) ? EINVAL : real(mx);
}

int pthread_mutex_trylock(pthread_mutex_t *mx)
{
	static mfn real;
	const char *m = mutant_of("pthread_mutex_init.");

	if (real == NULL)
		real = (mfn)dlsym(RTLD_NEXT, "pthread_mutex_trylock");
	return refused(m, mx) ? EINVAL : real(mx);
}

int pthread_mutex_unlock(pthread_mutex_t *mx)
{
	static mfn real;
	const char *m = mutant_of("pthread_mutex_init.");

	if (real == NULL)
		real = (mfn)dlsym(RTLD_NEXT, "pthread_mutex_unlock");
	return refused(m, mx) ? EINVAL : real(mx);
}
