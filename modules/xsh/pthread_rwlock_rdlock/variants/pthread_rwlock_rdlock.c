/*
 * pthread_rwlock_rdlock.c - conforming variants of the read-lock
 * functions, for the variant checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; see core/pcvariant.h. Each variant says why the
 * standard allows it.
 */
#define _GNU_SOURCE 1
#include <errno.h>
#include <pthread.h>
#include <stdatomic.h>
#include "pcvariant.h"

/*
 * "rd_max2": a lock takes at most two simultaneous read locks (all
 * threads together); a third rdlock or tryrdlock fails with EAGAIN. The
 * page makes the maximum implementation-defined and lets both functions
 * fail with [EAGAIN] beyond it; it states no minimum.
 *
 * Read locks are counted per lock; each thread also counts its own, so
 * that an unlock can be told apart from a writer's.
 */
#define NREC 16
static struct { void *l; int readers; } recs[NREC];
static atomic_flag guard = ATOMIC_FLAG_INIT;
static _Thread_local struct { void *l; int n; } mine[8];

static void take(void) { while (atomic_flag_test_and_set(&guard)) ; }
static void give(void) { atomic_flag_clear(&guard); }

static int *readers_of(void *l)
{
	int i;

	for (i = 0; i < NREC; i++)
		if (recs[i].l == l)
			return &recs[i].readers;
	for (i = 0; i < NREC; i++)
		if (recs[i].l == NULL) {
			recs[i].l = l;
			recs[i].readers = 0;
			return &recs[i].readers;
		}
	return &recs[0].readers;
}

static int *mine_of(void *l)
{
	int i;

	for (i = 0; i < 8; i++)
		if (mine[i].l == l)
			return &mine[i].n;
	for (i = 0; i < 8; i++)
		if (mine[i].l == NULL || mine[i].n == 0) {
			mine[i].l = l;
			mine[i].n = 0;
			return &mine[i].n;
		}
	return &mine[0].n;
}

/* Reserves a read lock (0) or refuses it (EAGAIN). */
static int reserve(const char *v, void *l)
{
	int *rd, r = 0;

	if (!IS("rd_max2"))
		return 0;
	take();
	rd = readers_of(l);
	if (*rd >= 2)
		r = EAGAIN;
	else
		++*rd;
	give();
	return r;
}

static void unreserve(const char *v, void *l)
{
	if (!IS("rd_max2"))
		return;
	take();
	--*readers_of(l);
	give();
}

static int read_lock(pthread_rwlock_t *l, int try)
{
	const char *v = variant_of("pthread_rwlock_rdlock.");
	int r;
	REAL(pthread_rwlock_rdlock);
	REAL(pthread_rwlock_tryrdlock);

	if ((r = reserve(v, l)) != 0)
		return r;
	r = try ? real_pthread_rwlock_tryrdlock(l) : real_pthread_rwlock_rdlock(l);
	if (r == 0)
		++*mine_of(l);
	else
		unreserve(v, l);
	return r;
}

int pthread_rwlock_rdlock(pthread_rwlock_t *l)
{
	return read_lock(l, 0);
}

int pthread_rwlock_tryrdlock(pthread_rwlock_t *l)
{
	return read_lock(l, 1);
}

int pthread_rwlock_unlock(pthread_rwlock_t *l)
{
	const char *v = variant_of("pthread_rwlock_rdlock.");
	int *n = mine_of(l), r;
	REAL(pthread_rwlock_unlock);

	r = real_pthread_rwlock_unlock(l);
	if (r == 0 && *n > 0) {
		--*n;
		unreserve(v, l);
	}
	return r;
}
