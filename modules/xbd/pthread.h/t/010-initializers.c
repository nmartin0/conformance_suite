/*
 * PTHREAD_MUTEX_INITIALIZER, PTHREAD_COND_INITIALIZER and
 * PTHREAD_RWLOCK_INITIALIZER initialise static objects (so they are
 * constant expressions) that the lock, signal and unlock functions then
 * accept without an init call.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.pthread.h.010
 */
#include "pc.h"
#include <errno.h>
#include <pthread.h>

static pthread_mutex_t mx = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cv = PTHREAD_COND_INITIALIZER;
static pthread_rwlock_t rw = PTHREAD_RWLOCK_INITIALIZER;

int main(void)
{
	int e;

	pc_begin("XBD.pthread.h.010");
	pc_test();
	e = pthread_mutex_lock(&mx);
	pc_check(e == 0, "locking a mutex with PTHREAD_MUTEX_INITIALIZER failed (%s)", pc_errname(e));
	e = pthread_mutex_unlock(&mx);
	pc_check(e == 0, "unlocking it failed (%s)", pc_errname(e));
	e = pthread_mutex_trylock(&mx);
	pc_check(e == 0, "trylock on the unlocked mutex failed (%s)", pc_errname(e));
	e = pthread_mutex_unlock(&mx);
	pc_check(e == 0, "unlocking it failed (%s)", pc_errname(e));
	e = pthread_cond_signal(&cv);
	pc_check(e == 0, "pthread_cond_signal on a PTHREAD_COND_INITIALIZER variable failed (%s)", pc_errname(e));
	e = pthread_cond_broadcast(&cv);
	pc_check(e == 0, "pthread_cond_broadcast on it failed (%s)", pc_errname(e));
	e = pthread_rwlock_rdlock(&rw);
	pc_check(e == 0, "read-locking a PTHREAD_RWLOCK_INITIALIZER lock failed (%s)", pc_errname(e));
	e = pthread_rwlock_rdlock(&rw);
	pc_check(e == 0, "a second read lock failed (%s)", pc_errname(e));
	e = pthread_rwlock_unlock(&rw);
	pc_check(e == 0, "unlocking failed (%s)", pc_errname(e));
	e = pthread_rwlock_unlock(&rw);
	pc_check(e == 0, "unlocking failed (%s)", pc_errname(e));
	e = pthread_rwlock_wrlock(&rw);
	pc_check(e == 0, "write-locking failed (%s)", pc_errname(e));
	e = pthread_rwlock_unlock(&rw);
	pc_check(e == 0, "unlocking failed (%s)", pc_errname(e));
	pc_pass();
}
