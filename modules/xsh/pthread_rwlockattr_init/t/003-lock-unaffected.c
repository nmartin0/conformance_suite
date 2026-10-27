/*
 * A read-write lock keeps working after the attributes object it was made from is destroyed.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_rwlockattr_init.003
 */
#include "pc.h"
#include <errno.h>
#include <pthread.h>

#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

static const char *rn(int r)
{
	return r == 0 ? "0" : pc_errname(r);
}

int main(void)
{
	pthread_rwlockattr_t a;
	pthread_rwlock_t l;
	int r;

	pc_begin("XSH.pthread_rwlockattr_init.003");
	NEED(pthread_rwlockattr_init(&a) == 0, "pthread_rwlockattr_init failed");
	NEED(pthread_rwlock_init(&l, &a) == 0, "pthread_rwlock_init failed");
	pc_test();
	r = pthread_rwlockattr_destroy(&a);
	pc_check(r == 0, "pthread_rwlockattr_destroy() returned %s", rn(r));
	r = pthread_rwlock_rdlock(&l);
	pc_check(r == 0, "first pthread_rwlock_rdlock() returned %s", rn(r));
	r = pthread_rwlock_rdlock(&l);
	pc_check(r == 0, "second pthread_rwlock_rdlock() returned %s", rn(r));
	r = pthread_rwlock_trywrlock(&l);
	pc_check(r == EBUSY, "pthread_rwlock_trywrlock() with read locks held returned %s, expected EBUSY", rn(r));
	r = pthread_rwlock_unlock(&l);
	pc_check(r == 0, "first pthread_rwlock_unlock() returned %s", rn(r));
	r = pthread_rwlock_unlock(&l);
	pc_check(r == 0, "second pthread_rwlock_unlock() returned %s", rn(r));
	r = pthread_rwlock_wrlock(&l);
	pc_check(r == 0, "pthread_rwlock_wrlock() returned %s", rn(r));
	r = pthread_rwlock_unlock(&l);
	pc_check(r == 0, "pthread_rwlock_unlock() of the write lock returned %s", rn(r));
	(void)pthread_rwlock_destroy(&l);
	pc_pass();
}
