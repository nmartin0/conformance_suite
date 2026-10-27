/*
 * A thread spinning on a lock gets it when the holder unlocks.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_spin_unlock.002
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

static pthread_spinlock_t sp;
static atomic_int started, done;
static int lock_result = -1;

static void *spinner(void *arg)
{
	(void)arg;
	atomic_store(&started, 1);
	lock_result = pthread_spin_lock(&sp);
	if (lock_result == 0)
		(void)pthread_spin_unlock(&sp);
	atomic_store(&done, 1);
	return NULL;
}

int main(void)
{
	pthread_t t;
	int r, i;

	pc_begin("XSH.pthread_spin_unlock.002");
	NEED(pthread_spin_init(&sp, PTHREAD_PROCESS_PRIVATE) == 0, "pthread_spin_init failed");
	NEED(pthread_spin_lock(&sp) == 0, "pthread_spin_lock failed");
	NEED(pthread_create(&t, NULL, spinner, NULL) == 0, "pthread_create failed");
	for (i = 0; i < 2000 && !atomic_load(&started); i++)
		sleep_ms(5);
	NEED(atomic_load(&started), "the spinning thread did not start");
	pc_test();
	sleep_ms(300);
	pc_check(!atomic_load(&done), "pthread_spin_lock() returned %s while the lock was held", rn(lock_result));
	r = pthread_spin_unlock(&sp);
	pc_check(r == 0, "pthread_spin_unlock() returned %s", rn(r));
	for (i = 0; i < 2000 && !atomic_load(&done); i++)
		sleep_ms(5);
	pc_check(atomic_load(&done), "the spinning thread did not get the lock within 10 s of the unlock");
	NEED(pthread_join(t, NULL) == 0, "pthread_join failed");
	pc_check(lock_result == 0, "the spinning pthread_spin_lock() returned %s, expected 0", rn(lock_result));
	pc_pass();
}
