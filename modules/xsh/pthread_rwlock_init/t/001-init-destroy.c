/*
 * pthread_rwlock_init() gives an unlocked lock that can be destroyed and initialized again.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_rwlock_init.001
 */
#include "pc.h"
#include <errno.h>
#include <pthread.h>

static const char *rn(int r)
{
	return r == 0 ? "0" : pc_errname(r);
}

static void check_unlocked(pthread_rwlock_t *l, const char *when)
{
	int r;

	r = pthread_rwlock_trywrlock(l);
	pc_check(r == 0, "%s: pthread_rwlock_trywrlock() returned %s", when, rn(r));
	r = pthread_rwlock_unlock(l);
	pc_check(r == 0, "%s: pthread_rwlock_unlock() of the write lock returned %s", when, rn(r));
	r = pthread_rwlock_tryrdlock(l);
	pc_check(r == 0, "%s: pthread_rwlock_tryrdlock() returned %s", when, rn(r));
	r = pthread_rwlock_unlock(l);
	pc_check(r == 0, "%s: pthread_rwlock_unlock() of the read lock returned %s", when, rn(r));
}

int main(void)
{
	pthread_rwlock_t l;
	int r;

	pc_begin("XSH.pthread_rwlock_init.001");
	pc_test();
	r = pthread_rwlock_init(&l, NULL);
	pc_check(r == 0, "pthread_rwlock_init(NULL attributes) returned %s", rn(r));
	check_unlocked(&l, "after init");
	r = pthread_rwlock_destroy(&l);
	pc_check(r == 0, "pthread_rwlock_destroy() returned %s", rn(r));
	r = pthread_rwlock_init(&l, NULL);
	pc_check(r == 0, "pthread_rwlock_init() after the destroy returned %s", rn(r));
	check_unlocked(&l, "after destroy and init");
	r = pthread_rwlock_destroy(&l);
	pc_check(r == 0, "second pthread_rwlock_destroy() returned %s", rn(r));
	pc_pass();
}
