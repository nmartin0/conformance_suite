/*
 * A thread blocked on a mutex or a read-write lock does not stop another
 * thread from running.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.9-threads.008
 *
 * The main thread holds the lock; one thread blocks taking it; a third
 * thread computes (three million loop steps) and must finish within 30 s.
 */
#include "pcth.h"

static pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
static pthread_rwlock_t rw = PTHREAD_RWLOCK_INITIALIZER;

static void *block_m(void *a)
{
	(void)a;
	flag_set(0, 1);
	(void)pthread_mutex_lock(&m);
	(void)pthread_mutex_unlock(&m);
	flag_set(1, 1);
	return NULL;
}

static void *block_rw(void *a)
{
	(void)a;
	flag_set(0, 1);
	(void)pthread_rwlock_wrlock(&rw);
	(void)pthread_rwlock_unlock(&rw);
	flag_set(1, 1);
	return NULL;
}

static void *compute(void *a)
{
	volatile unsigned long acc = 0;
	unsigned long k;

	(void)a;
	for (k = 0; k < 3000000UL; k++)
		acc += k;
	flag_set(2, 1);
	return NULL;
}

int main(void)
{
	pthread_t b, c;
	int round;

	pc_begin("XSH.2.9-threads.008");
	guard(150);
	pc_test();
	for (round = 0; round < 2; round++) {
		flag_set(0, 0);
		flag_set(1, 0);
		flag_set(2, 0);
		if (round == 0)
			NEED(pthread_mutex_lock(&m) == 0, "pthread_mutex_lock failed");
		else
			NEED(pthread_rwlock_wrlock(&rw) == 0, "pthread_rwlock_wrlock failed");
		b = start(round == 0 ? block_m : block_rw, NULL);
		NEED(wait_flag(0, 1, 10000), "the blocking thread did not start");
		sleep_ms(200);
		NEED(flag_get(1) == 0, "the lock was not held");
		c = start(compute, NULL);
		pc_check(wait_flag(2, 1, 30000), "%s: a thread that never waits did not finish its work in 30 s while another thread was blocked",
		    round == 0 ? "mutex" : "read-write lock");
		if (round == 0)
			NEED(pthread_mutex_unlock(&m) == 0, "pthread_mutex_unlock failed");
		else
			NEED(pthread_rwlock_unlock(&rw) == 0, "pthread_rwlock_unlock failed");
		NEED(wait_flag(1, 1, 10000), "the blocked thread did not get the lock after the unlock");
		NEED(pthread_join(b, NULL) == 0, "pthread_join failed");
		NEED(pthread_join(c, NULL) == 0, "pthread_join failed");
	}
	pc_pass();
}
