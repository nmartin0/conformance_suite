/*
 * pthread_mutex_lock.c - planted faults in pthread_mutex_lock(),
 * pthread_mutex_trylock() and pthread_mutex_unlock(), for mutation checks
 * (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "pthread_mutex_lock.<fault>" and
 * apply while PC_PHASE is "test". To plant faults that depend on the
 * mutex's type or owner, the library keeps its own record of each mutex
 * (type and robustness from pthread_mutex_init(), owner and lock count
 * from successful calls, and whether it is inconsistent), in every phase.
 */
#define _GNU_SOURCE 1
#include <dlfcn.h>
#include <errno.h>
#include <pthread.h>
#include <stdatomic.h>
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

typedef int (*mfn)(pthread_mutex_t *);

static int (*real_init)(pthread_mutex_t *, const pthread_mutexattr_t *);
static mfn real_lock, real_trylock, real_unlock, real_consistent;

static void resolve(void)
{
	if (real_lock != NULL)
		return;
	real_init = (int (*)(pthread_mutex_t *, const pthread_mutexattr_t *))dlsym(RTLD_NEXT, "pthread_mutex_init");
	real_trylock = (mfn)dlsym(RTLD_NEXT, "pthread_mutex_trylock");
	real_unlock = (mfn)dlsym(RTLD_NEXT, "pthread_mutex_unlock");
	real_consistent = (mfn)dlsym(RTLD_NEXT, "pthread_mutex_consistent");
	real_lock = (mfn)dlsym(RTLD_NEXT, "pthread_mutex_lock");
}

/* What the library knows of one mutex. */
struct rec {
	void *addr;
	int type, robust;
	int owned, count, inconsistent;
	pthread_t owner;
};

#define NREC 64
static struct rec recs[NREC];
static atomic_flag guard = ATOMIC_FLAG_INIT;

static void take(void) { while (atomic_flag_test_and_set(&guard)) ; }
static void give(void) { atomic_flag_clear(&guard); }

/* The record of addr (a new DEFAULT one if there is none); call under guard. */
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

/* A snapshot of the record. */
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

/* Records a successful acquisition (result 0 or EOWNERDEAD). */
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
	if (res == EOWNERDEAD)
		r->inconsistent = 1;
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

static void add_count(void *addr)
{
	take();
	rec_of(addr)->count++;
	give();
}

int pthread_mutex_init(pthread_mutex_t *mx, const pthread_mutexattr_t *attr)
{
	int type = PTHREAD_MUTEX_DEFAULT, robust = PTHREAD_MUTEX_STALLED, r;
	struct rec *rc;

	resolve();
	r = real_init(mx, attr);
	if (r != 0)
		return r;
	if (attr != NULL) {
		(void)pthread_mutexattr_gettype(attr, &type);
		(void)pthread_mutexattr_getrobust(attr, &robust);
	}
	take();
	rc = rec_of(mx);
	memset(rc, 0, sizeof *rc);
	rc->addr = mx;
	rc->type = type;
	rc->robust = robust == PTHREAD_MUTEX_ROBUST;
	give();
	return 0;
}

int pthread_mutex_consistent(pthread_mutex_t *mx)
{
	int r;

	resolve();
	r = real_consistent(mx);
	if (r == 0) {
		take();
		rec_of(mx)->inconsistent = 0;
		give();
	}
	return r;
}

/* After a real acquisition: the robust-mutex faults. */
static int after_acquire(const char *m, pthread_mutex_t *mx, int r, int is_try)
{
	if (r == EOWNERDEAD && IS("ownerdead_zero")) {
		(void)pthread_mutex_consistent(mx);
		return 0;
	}
	if (r == EOWNERDEAD && (IS("ownerdead_unlocked") || (is_try && IS("ownerdead_busy")))) {
		(void)pthread_mutex_consistent(mx);
		(void)real_unlock(mx);
		released(mx);
		return IS("ownerdead_busy") ? EBUSY : EOWNERDEAD;
	}
	if (r == ENOTRECOVERABLE && IS("notrec_ebusy"))
		return EBUSY;
	return r;
}

int pthread_mutex_lock(pthread_mutex_t *mx)
{
	const char *m = mutant_of("pthread_mutex_lock.");
	struct rec rc;
	int r;

	resolve();
	if (m == NULL) {
		r = real_lock(mx);
		acquired(mx, r);
		return r;
	}
	rc = look(mx);
	if (IS("lock_unlocked"))
		return 0;
	if (IS("lock_einval"))
		return EINVAL;
	if (IS("lock_nowait") || IS("lock_ebusy")) {
		r = real_trylock(mx);
		acquired(mx, r);
		return r == EBUSY && IS("lock_nowait") ? 0 : r;
	}
	if (IS("recursive_relock_edeadlk") && rc.type == PTHREAD_MUTEX_RECURSIVE && mine(&rc))
		return EDEADLK;
	if ((IS("normal_relock_ok") || IS("normal_relock_eagain")) && rc.type == PTHREAD_MUTEX_NORMAL && mine(&rc)) {
		if (IS("normal_relock_eagain"))
			return EAGAIN;
		add_count(mx);
		return 0;
	}
	if (IS("lock_eintr")) {
		/* waits by polling, and gives up when a signal interrupts the wait */
		for (;;) {
			struct timespec ts = { 0, 2000000L };
			r = real_trylock(mx);
			if (r != EBUSY) {
				acquired(mx, r);
				return r;
			}
			if (nanosleep(&ts, NULL) == -1 && errno == EINTR)
				return EINTR;
		}
	}
	r = real_lock(mx);
	acquired(mx, r);
	if (r == EDEADLK && rc.type == PTHREAD_MUTEX_ERRORCHECK) {
		if (IS("errchk_relock_ok"))
			return 0;
		if (IS("errchk_relock_ebusy"))
			return EBUSY;
	}
	return after_acquire(m, mx, r, 0);
}

int pthread_mutex_trylock(pthread_mutex_t *mx)
{
	const char *m = mutant_of("pthread_mutex_lock.");
	struct rec rc;
	int r;

	resolve();
	if (m == NULL) {
		r = real_trylock(mx);
		acquired(mx, r);
		return r;
	}
	rc = look(mx);
	if (IS("trylock_unlocked"))
		return 0;
	if (IS("trylock_blocks")) {
		r = real_lock(mx);
		acquired(mx, r);
		return r;
	}
	if (IS("recursive_trylock_busy") && rc.type == PTHREAD_MUTEX_RECURSIVE && mine(&rc))
		return EBUSY;
	r = real_trylock(mx);
	acquired(mx, r);
	if (r == 0 && IS("trylock_ebusy_free")) {
		(void)real_unlock(mx);
		released(mx);
		return EBUSY;
	}
	if (r == EBUSY && IS("trylock_zero"))
		return 0;
	if (r == EBUSY && IS("trylock_own_counts") && mine(&rc)) {
		add_count(mx);
		return 0;
	}
	return after_acquire(m, mx, r, 1);
}

int pthread_mutex_unlock(pthread_mutex_t *mx)
{
	const char *m = mutant_of("pthread_mutex_lock.");
	struct rec rc;
	int r;

	resolve();
	rc = look(mx);
	if (m != NULL) {
		if (IS("unlock_noop"))
			return 0;
		if (IS("unlock_eperm") && mine(&rc))
			return EPERM;
		if (IS("trylock_own_counts") && mine(&rc) && rc.count > 1 && rc.type != PTHREAD_MUTEX_RECURSIVE) {
			released(mx);
			return 0;
		}
		if (IS("recursive_no_count") && rc.type == PTHREAD_MUTEX_RECURSIVE && mine(&rc)) {
			int n = rc.count;
			while (n-- > 0 && real_unlock(mx) == 0)
				released(mx);
			return 0;
		}
		if (IS("notrec_recovers") && rc.inconsistent)
			(void)pthread_mutex_consistent(mx);
	}
	r = real_unlock(mx);
	if (r == 0)
		released(mx);
	if (m != NULL && r == EPERM) {
		if (IS("errchk_unlock_ok") && rc.type == PTHREAD_MUTEX_ERRORCHECK)
			return 0;
		if (IS("recursive_unlock_ok") && rc.type == PTHREAD_MUTEX_RECURSIVE)
			return 0;
		if (IS("robust_unlock_ok") && rc.robust)
			return 0;
		if (IS("unlock_eperm_einval"))
			return EINVAL;
	}
	return r;
}
