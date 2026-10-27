/*
 * Unlocking a write lock leaves the lock free.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_rwlock_unlock.002
 */
#include "pc.h"
#include <errno.h>
#include <pthread.h>

#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

static const char *rn(int r)
{
	return r == 0 ? "0" : pc_errname(r);
}

static pthread_rwlock_t l = PTHREAD_RWLOCK_INITIALIZER;

/* Other threads only try; they report through the return value. */
static void *try_rd(void *arg)
{
	int r = pthread_rwlock_tryrdlock(&l);

	(void)arg;
	if (r == 0)
		(void)pthread_rwlock_unlock(&l);
	return (void *)(long)r;
}

static void *try_wr(void *arg)
{
	int r = pthread_rwlock_trywrlock(&l);

	(void)arg;
	if (r == 0)
		(void)pthread_rwlock_unlock(&l);
	return (void *)(long)r;
}

static int in_thread(void *(*fn)(void *))
{
	pthread_t t;
	void *out;

	NEED(pthread_create(&t, NULL, fn, NULL) == 0, "pthread_create failed");
	NEED(pthread_join(t, &out) == 0, "pthread_join failed");
	return (int)(long)out;
}


int main(void)
{
	int r;

	pc_begin("XSH.pthread_rwlock_unlock.002");
	NEED(pthread_rwlock_wrlock(&l) == 0, "pthread_rwlock_wrlock failed");
	r = in_thread(try_rd);
	NEED(r == EBUSY, "with the write lock held, another thread's tryrdlock returned %s", rn(r));
	pc_test();
	r = pthread_rwlock_unlock(&l);
	pc_check(r == 0, "pthread_rwlock_unlock() of the write lock returned %s", rn(r));
	r = in_thread(try_rd);
	pc_check(r == 0, "after unlocking the write lock, another thread's tryrdlock returned %s, expected 0", rn(r));
	r = in_thread(try_wr);
	pc_check(r == 0, "after unlocking the write lock, another thread's trywrlock returned %s, expected 0", rn(r));
	pc_pass();
}
