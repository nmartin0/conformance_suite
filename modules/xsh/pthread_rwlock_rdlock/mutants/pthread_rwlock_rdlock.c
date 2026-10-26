/*
 * pthread_rwlock_rdlock.c - planted faults in pthread_rwlock_rdlock() and
 * pthread_rwlock_tryrdlock(), for mutation checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "pthread_rwlock_rdlock.<fault>" and
 * apply while PC_PHASE is "test". Each thread's read locks per lock are
 * counted, in every phase, for the faults that depend on them.
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

typedef int (*lfn)(pthread_rwlock_t *);
static lfn real_rd, real_tryrd, real_wr, real_trywr, real_unlock;

static void resolve(void)
{
	if (real_unlock != NULL)
		return;
	real_rd = (lfn)dlsym(RTLD_NEXT, "pthread_rwlock_rdlock");
	real_tryrd = (lfn)dlsym(RTLD_NEXT, "pthread_rwlock_tryrdlock");
	real_wr = (lfn)dlsym(RTLD_NEXT, "pthread_rwlock_wrlock");
	real_trywr = (lfn)dlsym(RTLD_NEXT, "pthread_rwlock_trywrlock");
	real_unlock = (lfn)dlsym(RTLD_NEXT, "pthread_rwlock_unlock");
}

/* This thread's read locks, per lock. */
#define NREC 8
static _Thread_local struct { void *l; int n; } mine[NREC];

static int *count_of(void *l)
{
	int i;

	for (i = 0; i < NREC; i++)
		if (mine[i].l == l)
			return &mine[i].n;
	for (i = 0; i < NREC; i++)
		if (mine[i].l == NULL || mine[i].n == 0) {
			mine[i].l = l;
			mine[i].n = 0;
			return &mine[i].n;
		}
	return &mine[0].n;
}

static int got_read(void *l, int r)
{
	if (r == 0)
		++*count_of(l);
	return r;
}

int pthread_rwlock_rdlock(pthread_rwlock_t *l)
{
	const char *m = mutant_of("pthread_rwlock_rdlock.");
	int r;

	resolve();
	if (IS("rd_exclusive"))
		return got_read(l, real_wr(l));
	if (IS("rd_nowait") || IS("rd_ebusy")) {
		r = got_read(l, real_tryrd(l));
		return r == EBUSY && IS("rd_nowait") ? 0 : r;
	}
	if (IS("rd_relock_edeadlk") && *count_of(l) > 0)
		return EDEADLK;
	if (IS("rd_eintr")) {
		/* waits by polling, and gives up when a signal interrupts the wait */
		for (;;) {
			struct timespec ts = { 0, 2000000L };
			r = real_tryrd(l);
			if (r != EBUSY)
				return got_read(l, r);
			if (nanosleep(&ts, NULL) == -1 && errno == EINTR)
				return EINTR;
		}
	}
	return got_read(l, real_rd(l));
}

int pthread_rwlock_tryrdlock(pthread_rwlock_t *l)
{
	const char *m = mutant_of("pthread_rwlock_rdlock.");
	int r;

	resolve();
	if (IS("tryrd_blocks"))
		return got_read(l, real_rd(l));
	if (IS("tryrd_busy_readers")) {
		/* a read lock only when the lock is entirely free */
		if (real_trywr(l) != 0)
			return EBUSY;
		(void)real_unlock(l);
	}
	r = got_read(l, real_tryrd(l));
	if (r == EBUSY && IS("tryrd_ignores_writer"))
		return 0;
	if (r == EBUSY && IS("tryrd_eagain"))
		return EAGAIN;
	return r;
}

int pthread_rwlock_unlock(pthread_rwlock_t *l)
{
	const char *m = mutant_of("pthread_rwlock_rdlock.");
	int *n, r;

	resolve();
	n = count_of(l);
	if (IS("rd_unlock_all") && *n > 1) {
		/* one unlock drops every read lock this thread holds */
		while (*n > 0 && real_unlock(l) == 0)
			--*n;
		return 0;
	}
	r = real_unlock(l);
	if (r == 0 && *n > 0)
		--*n;
	return r;
}
