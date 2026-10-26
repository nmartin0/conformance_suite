/*
 * A new thread has a CPU-time clock that starts at zero.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_create.010
 * MUTANT    pthread_create.cpuclock_inherited
 * MUTANT    pthread_create.cpuclock_missing
 * VARIANT   pthread_create.runs_late
 */
#include "pc.h"
#include <errno.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

static pthread_mutex_t mx = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cv = PTHREAD_COND_INITIALIZER;

/* Sets *p to v and wakes every waiter. */
static void post(int *p, int v)
{
	(void)pthread_mutex_lock(&mx);
	*p = v;
	(void)pthread_cond_broadcast(&cv);
	(void)pthread_mutex_unlock(&mx);
}

/* Waits until *p is v. */
static void await(int *p, int v)
{
	(void)pthread_mutex_lock(&mx);
	while (*p != v)
		(void)pthread_cond_wait(&cv, &mx);
	(void)pthread_mutex_unlock(&mx);
}

/* The value of *p, read under the lock. */
static int peek(int *p)
{
	int v;

	(void)pthread_mutex_lock(&mx);
	v = *p;
	(void)pthread_mutex_unlock(&mx);
	return v;
}

static void sleep_ms(long ms)
{
	struct timespec ts;

	ts.tv_sec = ms / 1000;
	ts.tv_nsec = (ms % 1000) * 1000000L;
	while (nanosleep(&ts, &ts) == -1 && errno == EINTR)
		;
}

#if defined(_POSIX_THREAD_CPUTIME) && _POSIX_THREAD_CPUTIME >= 0 && defined(CLOCK_THREAD_CPUTIME_ID)
#define BURN_NS 300000000L

static long long first_ns = -1;
static int have_first, release;

static long long ns_of(struct timespec ts)
{
	return (long long)ts.tv_sec * 1000000000LL + ts.tv_nsec;
}

static void *start(void *arg)
{
	struct timespec ts;
	long long v = -2;

	(void)arg;
	if (clock_gettime(CLOCK_THREAD_CPUTIME_ID, &ts) == 0)
		v = ns_of(ts);
	(void)pthread_mutex_lock(&mx);
	first_ns = v;
	(void)pthread_mutex_unlock(&mx);
	post(&have_first, 1);
	await(&release, 1);	/* alive while its clock is read from outside */
	return NULL;
}

int main(void)
{
	struct timespec ts;
	volatile unsigned long spin = 0;
	clockid_t cid;
	pthread_t t;
	long long v;
	int r, g;

	pc_begin("XSH.pthread_create.010");
	if (sysconf(_SC_THREAD_CPUTIME) <= 0)
		pc_unsupported("_SC_THREAD_CPUTIME is not supported");
	do {
		for (r = 0; r < 100000; r++)
			spin++;
		pc_check(clock_gettime(CLOCK_THREAD_CPUTIME_ID, &ts) == 0, "clock_gettime failed: %s", pc_errname(errno));
	} while (ns_of(ts) < BURN_NS);
	pc_test();
	r = pthread_create(&t, NULL, start, NULL);
	if (r != 0)
		pc_unresolved("pthread_create failed: %s", pc_errname(r));
	await(&have_first, 1);
	r = pthread_getcpuclockid(t, &cid);
	g = r == 0 ? clock_gettime(cid, &ts) : -1;
	post(&release, 1);
	(void)pthread_join(t, NULL);
	pc_check(r == 0, "pthread_getcpuclockid for the new thread failed: %s", pc_errname(r));
	pc_check(g == 0, "clock_gettime on the new thread's clock failed: %s", pc_errname(errno));
	(void)pthread_mutex_lock(&mx);
	v = first_ns;
	(void)pthread_mutex_unlock(&mx);
	pc_check(v != -2, "clock_gettime(CLOCK_THREAD_CPUTIME_ID) failed in the new thread");
	pc_check(v < BURN_NS, "the new thread's CPU-time clock read %lld ns at its start, not less than the %ld ns "
		 "its creator had used: it did not start at zero", v, BURN_NS);
	pc_pass();
}
#else
int main(void)
{
	pc_begin("XSH.pthread_create.010");
	pc_unsupported("_POSIX_THREAD_CPUTIME or CLOCK_THREAD_CPUTIME_ID is not defined");
}
#endif
