/*
 * pthread_mutex_lock.c - conforming variants of the mutex functions, for
 * the variant checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; see core/pcvariant.h. Each variant says why the
 * standard allows it. The library keeps its own record of each mutex
 * (type and robustness from pthread_mutex_init(), owner and lock count).
 */
#define _GNU_SOURCE 1
#include <errno.h>
#include <pthread.h>
#include <stdatomic.h>
#include "pcvariant.h"

struct rec {
	void *addr;
	int type, owned, count;
	pthread_t owner;
};

#define NREC 64
static struct rec recs[NREC];
static atomic_flag guard = ATOMIC_FLAG_INIT;

static void take(void) { while (atomic_flag_test_and_set(&guard)) ; }
static void give(void) { atomic_flag_clear(&guard); }

static struct rec *rec_of(void *addr)
{
	int i, free_slot = -1;

	for (i = 0; i < NREC; i++) {
		if (recs[i].addr == addr)
			return &recs[i];
		if (recs[i].addr == NULL && free_slot < 0)
			free_slot = i;
	}
	if (free_slot < 0)
		free_slot = 0;
	memset(&recs[free_slot], 0, sizeof recs[free_slot]);
	recs[free_slot].addr = addr;
	recs[free_slot].type = PTHREAD_MUTEX_DEFAULT;
	return &recs[free_slot];
}

static struct rec look(void *addr)
{
	struct rec r;

	take();
	r = *rec_of(addr);
	give();
	return r;
}

static int mine(const struct rec *r)
{
	return r->owned && pthread_equal(r->owner, pthread_self());
}

static void acquired(void *addr, int res)
{
	struct rec *r;

	if (res != 0 && res != EOWNERDEAD)
		return;
	take();
	r = rec_of(addr);
	if (mine(r))
		r->count++;
	else {
		r->owned = 1;
		r->owner = pthread_self();
		r->count = 1;
	}
	give();
}

static void released(void *addr)
{
	struct rec *r;

	take();
	r = rec_of(addr);
	if (r->count > 0 && --r->count == 0)
		r->owned = 0;
	give();
}

/*
 * "robust_private_einval": pthread_mutex_init() refuses a robust mutex
 * that is not process-shared with EINVAL, a "may fail" error of
 * pthread_mutex_init(); its RATIONALE requires robust mutexes only for
 * process-shared ones.
 */
int pthread_mutex_init(pthread_mutex_t *mx, const pthread_mutexattr_t *attr)
{
	const char *v = variant_of("pthread_mutex_lock.");
	int type = PTHREAD_MUTEX_DEFAULT, robust = PTHREAD_MUTEX_STALLED, ps = PTHREAD_PROCESS_PRIVATE, r;
	struct rec *rc;
	REAL(pthread_mutex_init);

	if (attr != NULL) {
		(void)pthread_mutexattr_gettype(attr, &type);
		(void)pthread_mutexattr_getrobust(attr, &robust);
		(void)pthread_mutexattr_getpshared(attr, &ps);
	}
	if (IS("robust_private_einval") && robust == PTHREAD_MUTEX_ROBUST && ps != PTHREAD_PROCESS_SHARED)
		return EINVAL;
	r = real_pthread_mutex_init(mx, attr);
	if (r != 0)
		return r;
	take();
	rc = rec_of(mx);
	memset(rc, 0, sizeof *rc);
	rc->addr = mx;
	rc->type = type;
	give();
	return 0;
}

/*
 * "recursive_max2": a RECURSIVE mutex takes at most two locks; a third
 * lock or trylock by the owner fails with EAGAIN. The page's [EAGAIN]
 * error ("maximum number of recursive locks ... exceeded") states no
 * minimum for that maximum.
 *
 * "recursive_max1": the same with a maximum of one lock: the owner's
 * second lock or trylock fails with EAGAIN (no minimum is stated).
 *
 * "normal_relock_edeadlk": relocking a NORMAL mutex is reported with
 * EDEADLK instead of hanging, under pthread_mutex_lock()'s "may fail"
 * [EDEADLK] (a deadlock condition was detected).
 */
static int refused(const char *v, const struct rec *rc)
{
	if (IS("recursive_max2") && rc->type == PTHREAD_MUTEX_RECURSIVE && mine(rc) && rc->count >= 2)
		return EAGAIN;
	if (IS("recursive_max1") && rc->type == PTHREAD_MUTEX_RECURSIVE && mine(rc) && rc->count >= 1)
		return EAGAIN;
	if (IS("normal_relock_edeadlk") && rc->type == PTHREAD_MUTEX_NORMAL && mine(rc))
		return EDEADLK;
	return 0;
}

int pthread_mutex_lock(pthread_mutex_t *mx)
{
	const char *v = variant_of("pthread_mutex_lock.");
	struct rec rc = look(mx);
	int r;
	REAL(pthread_mutex_lock);

	if ((r = refused(v, &rc)) != 0)
		return r;
	r = real_pthread_mutex_lock(mx);
	acquired(mx, r);
	return r;
}

int pthread_mutex_trylock(pthread_mutex_t *mx)
{
	const char *v = variant_of("pthread_mutex_lock.");
	struct rec rc = look(mx);
	int r;
	REAL(pthread_mutex_trylock);

	if ((IS("recursive_max2") || IS("recursive_max1")) && (r = refused(v, &rc)) == EAGAIN)
		return r;
	r = real_pthread_mutex_trylock(mx);
	acquired(mx, r);
	return r;
}

int pthread_mutex_unlock(pthread_mutex_t *mx)
{
	int r;
	REAL(pthread_mutex_unlock);

	r = real_pthread_mutex_unlock(mx);
	if (r == 0)
		released(mx);
	return r;
}
