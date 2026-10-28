/*
 * A process-shared condition variable and barrier work through another
 * mapping of the memory they are in.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.9-threads.036
 *
 * A file is mapped twice; a mutex, a condition variable and a barrier for
 * two threads are initialized through the first mapping. A thread waits on
 * the condition variable through the first mapping and is woken by a
 * signal sent through the second; a thread waits at the barrier through
 * the first mapping and the main thread arrives through the second.
 */
#include "pcth.h"
#include <fcntl.h>
#include <sys/mman.h>

#define OFF_MX 0
#define OFF_CV 256
#define OFF_BAR 512
#define OFF_PRED 1024

static unsigned char *m1, *m2;

static pthread_mutex_t *mx(unsigned char *b) { return (pthread_mutex_t *)(void *)(b + OFF_MX); }
static pthread_cond_t *cv(unsigned char *b) { return (pthread_cond_t *)(void *)(b + OFF_CV); }
static pthread_barrier_t *bar(unsigned char *b) { return (pthread_barrier_t *)(void *)(b + OFF_BAR); }
static volatile int *pred(unsigned char *b) { return (volatile int *)(void *)(b + OFF_PRED); }

static void *cond_waiter(void *a)
{
	(void)a;
	(void)pthread_mutex_lock(mx(m1));
	flag_set(0, 1);
	while (!*pred(m1))
		(void)pthread_cond_wait(cv(m1), mx(m1));
	(void)pthread_mutex_unlock(mx(m1));
	flag_set(1, 1);
	return NULL;
}

static void *bar_waiter(void *a)
{
	int r;

	(void)a;
	flag_set(2, 1);
	r = pthread_barrier_wait(bar(m1));
	flag_set(3, r == PTHREAD_BARRIER_SERIAL_THREAD ? 2 : r == 0 ? 1 : 3);
	return NULL;
}

int main(void)
{
	pthread_mutexattr_t ma;
	pthread_condattr_t ca;
	pthread_barrierattr_t ba;
	pthread_t t;
	int fd, r, mine;
	long ps = sysconf(_SC_PAGESIZE);

	pc_begin("XSH.2.9-threads.036");
	guard(120);
	fd = open("shm", O_RDWR | O_CREAT | O_EXCL, 0600);
	NEED(fd != -1, "open failed");
	NEED(ftruncate(fd, ps) == 0, "ftruncate failed");
	m1 = mmap(NULL, (size_t)ps, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	m2 = mmap(NULL, (size_t)ps, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	NEED(m1 != MAP_FAILED && m2 != MAP_FAILED, "mmap failed");
	NEED(m1 != m2, "the two mappings have the same address");
	NEED(pthread_mutexattr_init(&ma) == 0 && pthread_mutexattr_setpshared(&ma, PTHREAD_PROCESS_SHARED) == 0, "mutex attributes failed");
	NEED(pthread_condattr_init(&ca) == 0 && pthread_condattr_setpshared(&ca, PTHREAD_PROCESS_SHARED) == 0, "condition attributes failed");
	NEED(pthread_barrierattr_init(&ba) == 0 && pthread_barrierattr_setpshared(&ba, PTHREAD_PROCESS_SHARED) == 0, "barrier attributes failed");
	r = pthread_mutex_init(mx(m1), &ma);
	NEED(r == 0, "pthread_mutex_init failed: %s", pc_errname(r));
	r = pthread_cond_init(cv(m1), &ca);
	NEED(r == 0, "pthread_cond_init failed: %s", pc_errname(r));
	r = pthread_barrier_init(bar(m1), &ba, 2);
	NEED(r == 0, "pthread_barrier_init failed: %s", pc_errname(r));
	*pred(m1) = 0;
	pc_test();

	t = start(cond_waiter, NULL);
	NEED(wait_flag(0, 1, 10000), "the waiter did not start");
	sleep_ms(200);
	NEED(pthread_mutex_lock(mx(m2)) == 0, "pthread_mutex_lock through the second mapping failed");
	*pred(m2) = 1;
	r = pthread_cond_signal(cv(m2));
	pc_check(r == 0, "pthread_cond_signal through the second mapping returned %s", r == 0 ? "0" : pc_errname(r));
	NEED(pthread_mutex_unlock(mx(m2)) == 0, "pthread_mutex_unlock through the second mapping failed");
	pc_check(wait_flag(1, 1, 10000), "a thread waiting on the condition variable through the first mapping was not woken by a signal sent through the second");
	NEED(join_within(t, 10000, NULL), "the waiter did not end");

	t = start(bar_waiter, NULL);
	NEED(wait_flag(2, 1, 10000), "the barrier thread did not start");
	sleep_ms(200);
	pc_check(flag_get(3) == 0, "a thread passed the barrier before the second thread arrived");
	r = pthread_barrier_wait(bar(m2));
	mine = r == PTHREAD_BARRIER_SERIAL_THREAD ? 2 : r == 0 ? 1 : 3;
	pc_check(mine != 3, "pthread_barrier_wait through the second mapping returned %s", pc_errname(r));
	pc_check(wait_flag(3, 1, 10000), "a thread waiting at the barrier through the first mapping was not released by the arrival through the second");
	NEED(join_within(t, 10000, NULL), "the barrier thread did not end");
	pc_check(flag_get(3) != 3, "pthread_barrier_wait through the first mapping returned an error");
	pc_check(mine + flag_get(3) == 3, "of the two threads released by the barrier exactly one must get PTHREAD_BARRIER_SERIAL_THREAD");
	pc_pass();
}
