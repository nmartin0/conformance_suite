/*
 * pthread_cond_signal.c - planted faults in pthread_cond_signal() and
 * pthread_cond_broadcast(), for mutation checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "pthread_cond_signal.<fault>" and
 * apply only while PC_PHASE is "test" (as set by pc.c in this process).
 * The waits are wrapped too, to keep the count a faulty signal needs.
 */
#define _GNU_SOURCE 1
#include <dlfcn.h>
#include <errno.h>
#include <pthread.h>
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

/*
 * coalesce: a signal is dropped while an earlier one's wakeup is still
 * outstanding (the woken thread has not yet returned from its wait), as
 * with a single "wakeup pending" flag per condition variable.
 */
static pthread_mutex_t pend_lock = PTHREAD_MUTEX_INITIALIZER;
static int pending;

int pthread_cond_signal(pthread_cond_t *cv)
{
	const char *m = mutant_of("pthread_cond_signal.");
	int drop = 0;
	REAL(pthread_cond_signal);

	if (IS("signal_noop"))
		return 0;
	if (IS("coalesce")) {
		(void)pthread_mutex_lock(&pend_lock);
		drop = pending > 0;
		if (!drop)
			pending++;
		(void)pthread_mutex_unlock(&pend_lock);
		if (drop)
			return 0;
	}
	return real_pthread_cond_signal(cv);
}

int pthread_cond_broadcast(pthread_cond_t *cv)
{
	const char *m = mutant_of("pthread_cond_signal.");
	REAL(pthread_cond_broadcast);
	REAL(pthread_cond_signal);

	if (IS("broadcast_one"))
		return real_pthread_cond_signal(cv);
	return real_pthread_cond_broadcast(cv);
}

static int after_wait(const char *m, pthread_mutex_t *mx, int r)
{
	if (IS("coalesce") && r == 0) {
		(void)pthread_mutex_lock(&pend_lock);
		if (pending > 0)
			pending--;
		(void)pthread_mutex_unlock(&pend_lock);
	}
	if (IS("returns_unlocked") && r == 0)
		(void)pthread_mutex_unlock(mx);	/* the woken thread does not get the mutex back */
	return r;
}

int pthread_cond_wait(pthread_cond_t *restrict cv, pthread_mutex_t *restrict mx)
{
	const char *m = mutant_of("pthread_cond_signal.");
	REAL(pthread_cond_wait);

	return after_wait(m, mx, real_pthread_cond_wait(cv, mx));
}

int pthread_cond_timedwait(pthread_cond_t *restrict cv, pthread_mutex_t *restrict mx,
			   const struct timespec *restrict abstime)
{
	const char *m = mutant_of("pthread_cond_signal.");
	REAL(pthread_cond_timedwait);

	return after_wait(m, mx, real_pthread_cond_timedwait(cv, mx, abstime));
}
