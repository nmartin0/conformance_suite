/*
 * PTHREAD_RWLOCK_INITIALIZER gives an unlocked, working lock.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_rwlock_init.002
 */
#include "pc.h"
#include <errno.h>
#include <pthread.h>

#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

static pthread_rwlock_t l = PTHREAD_RWLOCK_INITIALIZER;

static void *tryrd(void *arg)
{
	int r = pthread_rwlock_tryrdlock(&l);

	(void)arg;
	if (r == 0)
		(void)pthread_rwlock_unlock(&l);
	return (void *)(long)r;
}

static int other_tryrd(void)
{
	pthread_t t;
	void *out;

	NEED(pthread_create(&t, NULL, tryrd, NULL) == 0, "pthread_create failed");
	NEED(pthread_join(t, &out) == 0, "pthread_join failed");
	return (int)(long)out;
}

static const char *rn(int r)
{
	return r == 0 ? "0" : pc_errname(r);
}

int main(void)
{
	int r;

	pc_begin("XSH.pthread_rwlock_init.002");
	pc_test();
	r = pthread_rwlock_wrlock(&l);
	pc_check(r == 0, "pthread_rwlock_wrlock() of a statically initialized lock returned %s", rn(r));
	r = other_tryrd();
	pc_check(r == EBUSY, "another thread's tryrdlock with the write lock held returned %s, expected EBUSY", rn(r));
	r = pthread_rwlock_unlock(&l);
	pc_check(r == 0, "pthread_rwlock_unlock() returned %s", rn(r));
	r = other_tryrd();
	pc_check(r == 0, "another thread's tryrdlock after the unlock returned %s, expected 0", rn(r));
	pc_pass();
}
