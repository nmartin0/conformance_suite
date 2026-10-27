/*
 * A blocked writer gets the lock after the last reader unlocks.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_rwlock_unlock.003
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





static atomic_int started, acquired;
static int wr_result = -1;

static void *writer(void *arg)
{
	(void)arg;
	atomic_store(&started, 1);
	wr_result = pthread_rwlock_wrlock(&l);
	if (wr_result == 0)
		(void)pthread_rwlock_unlock(&l);
	atomic_store(&acquired, 1);
	return NULL;
}

int main(void)
{
	pthread_t t;
	int r, i;

	pc_begin("XSH.pthread_rwlock_unlock.003");
	NEED(pthread_rwlock_rdlock(&l) == 0, "first pthread_rwlock_rdlock failed");
	NEED(pthread_rwlock_rdlock(&l) == 0, "second pthread_rwlock_rdlock failed");
	NEED(pthread_create(&t, NULL, writer, NULL) == 0, "pthread_create failed");
	for (i = 0; i < 2000 && !atomic_load(&started); i++)
		sleep_ms(5);
	NEED(atomic_load(&started), "the writer did not start");
	pc_test();
	sleep_ms(200);
	pc_check(!atomic_load(&acquired), "pthread_rwlock_wrlock() returned %s while two read locks were held", rn(wr_result));
	r = pthread_rwlock_unlock(&l);
	pc_check(r == 0, "first pthread_rwlock_unlock() returned %s", rn(r));
	sleep_ms(200);
	pc_check(!atomic_load(&acquired), "pthread_rwlock_wrlock() returned %s while one read lock was held", rn(wr_result));
	r = pthread_rwlock_unlock(&l);
	pc_check(r == 0, "second pthread_rwlock_unlock() returned %s", rn(r));
	for (i = 0; i < 2000 && !atomic_load(&acquired); i++)
		sleep_ms(5);
	pc_check(atomic_load(&acquired), "the blocked pthread_rwlock_wrlock() did not return within 10 s of the last unlock");
	NEED(pthread_join(t, NULL) == 0, "pthread_join failed");
	pc_check(wr_result == 0, "the blocked pthread_rwlock_wrlock() returned %s, expected 0", rn(wr_result));
	pc_pass();
}
