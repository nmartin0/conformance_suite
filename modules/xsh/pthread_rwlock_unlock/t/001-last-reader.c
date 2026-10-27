/*
 * A read-write lock stays read-locked until the last read lock is released.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_rwlock_unlock.001
 */
#include "pc.h"
#include <errno.h>
#include <pthread.h>
#include <stdatomic.h>
#include <time.h>

#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

static void sleep_ms(long ms)
{
	struct timespec ts;

	ts.tv_sec = ms / 1000;
	ts.tv_nsec = (ms % 1000) * 1000000L;
	while (nanosleep(&ts, &ts) == -1 && errno == EINTR)
		;
}

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


/* A reader thread: takes a read lock, tells the main thread, releases on request. */
static atomic_int got, release, unlocked;
static int unlock_result = -1;

static void *reader(void *arg)
{
	(void)arg;
	if (pthread_rwlock_rdlock(&l) != 0)
		return NULL;
	atomic_store(&got, 1);
	while (!atomic_load(&release))
		sleep_ms(2);
	unlock_result = pthread_rwlock_unlock(&l);
	atomic_store(&unlocked, 1);
	return NULL;
}

int main(void)
{
	pthread_t t;
	int r, i;

	pc_begin("XSH.pthread_rwlock_unlock.001");
	NEED(pthread_rwlock_rdlock(&l) == 0, "pthread_rwlock_rdlock failed");
	NEED(pthread_create(&t, NULL, reader, NULL) == 0, "pthread_create failed");
	for (i = 0; i < 2000 && !atomic_load(&got); i++)
		sleep_ms(5);
	NEED(atomic_load(&got), "the second reader did not get its read lock");
	pc_test();
	r = pthread_rwlock_unlock(&l);	/* the main thread's read lock */
	pc_check(r == 0, "first pthread_rwlock_unlock() returned %s", rn(r));
	r = in_thread(try_wr);
	pc_check(r == EBUSY, "with one read lock left, a try for a write lock returned %s, expected EBUSY", rn(r));
	r = in_thread(try_rd);
	pc_check(r == 0, "with one read lock left, a try for a read lock returned %s, expected 0", rn(r));
	atomic_store(&release, 1);
	for (i = 0; i < 2000 && !atomic_load(&unlocked); i++)
		sleep_ms(5);
	pc_check(atomic_load(&unlocked), "the second reader did not unlock");
	NEED(pthread_join(t, NULL) == 0, "pthread_join failed");
	pc_check(unlock_result == 0, "second pthread_rwlock_unlock() returned %s", rn(unlock_result));
	r = in_thread(try_wr);
	pc_check(r == 0, "after the last read lock was released, a try for a write lock returned %s, expected 0", rn(r));
	pc_pass();
}
