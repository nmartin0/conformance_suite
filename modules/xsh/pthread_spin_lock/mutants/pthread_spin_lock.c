/*
 * pthread_spin_lock.c - planted faults in pthread_spin_lock() and
 * pthread_spin_trylock(), for mutation checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "pthread_spin_lock.<fault>" and
 * apply while PC_PHASE is "test". The holder of each spin lock is
 * recorded, in every phase, for the fault that depends on it.
 */
#define _GNU_SOURCE 1
#include <dlfcn.h>
#include <errno.h>
#include <pthread.h>
#include <stdatomic.h>
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

typedef int (*sfn)(pthread_spinlock_t *);
static sfn real_lock, real_trylock, real_unlock;

static void resolve(void)
{
	if (real_unlock != NULL)
		return;
	real_lock = (sfn)dlsym(RTLD_NEXT, "pthread_spin_lock");
	real_trylock = (sfn)dlsym(RTLD_NEXT, "pthread_spin_trylock");
	real_unlock = (sfn)dlsym(RTLD_NEXT, "pthread_spin_unlock");
}

/* The spin locks this thread holds. */
static _Thread_local void *held[8];

static int got(void *l, int r)
{
	int i;

	if (r == 0)
		for (i = 0; i < 8; i++)
			if (held[i] == NULL) {
				held[i] = l;
				break;
			}
	return r;
}

static int holds(void *l)
{
	int i;

	for (i = 0; i < 8; i++)
		if (held[i] == l)
			return 1;
	return 0;
}

int pthread_spin_lock(pthread_spinlock_t *l)
{
	const char *m = mutant_of("pthread_spin_lock.");
	int r;

	resolve();
	if (IS("lock_noop"))
		return 0;
	if (IS("lock_einval"))
		return EINVAL;
	if (IS("lock_nowait") || IS("lock_ebusy")) {
		r = got((void *)l, real_trylock(l));
		return r == EBUSY && IS("lock_nowait") ? 0 : r;
	}
	if (IS("lock_eintr")) {
		/* waits by polling, and gives up when a signal interrupts the wait */
		for (;;) {
			struct timespec ts = { 0, 2000000L };
			r = real_trylock(l);
			if (r != EBUSY)
				return got((void *)l, r);
			if (nanosleep(&ts, NULL) == -1 && errno == EINTR)
				return EINTR;
		}
	}
	return got((void *)l, real_lock(l));
}

int pthread_spin_trylock(pthread_spinlock_t *l)
{
	const char *m = mutant_of("pthread_spin_lock.");
	int r;

	resolve();
	if (IS("trylock_noop"))
		return 0;
	if (IS("trylock_own_ok") && holds((void *)l))
		return 0;
	r = got((void *)l, real_trylock(l));
	if (r == EBUSY && IS("trylock_wrong_errno"))
		return EDEADLK;
	return r;
}

int pthread_spin_unlock(pthread_spinlock_t *l)
{
	int i, r;

	resolve();
	r = real_unlock(l);
	if (r == 0)
		for (i = 0; i < 8; i++)
			if (held[i] == (void *)l) {
				held[i] = NULL;
				break;
			}
	return r;
}
