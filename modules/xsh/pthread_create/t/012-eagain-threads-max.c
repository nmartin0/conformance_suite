/*
 * pthread_create() fails with EAGAIN rather than exceed
 * {PTHREAD_THREADS_MAX}.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_create.012
 * MUTANT    pthread_create.limit_enomem
 * VARIANT   pthread_create.threads_max_64
 */
#include "pc.h"
#include <errno.h>
#include <limits.h>
#include <pthread.h>
#include <stdlib.h>
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

#define CAP 4096

static int release;

static void *start(void *arg)
{
	(void)arg;
	await(&release, 1);
	return NULL;
}

int main(void)
{
	pthread_attr_t a;
	pthread_t *t;
	long max = sysconf(_SC_THREAD_THREADS_MAX);
	long i, made = 0, at = -1;
	int r, first = 0;

	pc_begin("XSH.pthread_create.012");
	if (max < 0)
		pc_notinuse("sysconf(_SC_THREAD_THREADS_MAX) reports no limit");
	if (max > CAP)
		pc_notinuse("{PTHREAD_THREADS_MAX} is %ld, too many threads to create here", max);
	t = malloc(sizeof *t * (size_t)(max + 1));
	pc_check(t != NULL, "malloc failed");
	pc_check(pthread_attr_init(&a) == 0, "pthread_attr_init failed");
#ifdef PTHREAD_STACK_MIN
	/* Small stacks keep memory use down; the limit is on threads, not memory. */
	(void)pthread_attr_setstacksize(&a, PTHREAD_STACK_MIN > 65536 ? PTHREAD_STACK_MIN : 65536);
#endif
	pc_test();
	for (i = 0; i <= max; i++) {
		r = pthread_create(&t[made], &a, start, NULL);
		if (r != 0) {
			first = r;
			at = i;
			break;
		}
		made++;
	}
	post(&release, 1);
	for (i = 0; i < made; i++)
		(void)pthread_join(t[i], NULL);
	pc_check(at >= 0, "%ld threads were created besides the initial one, more than {PTHREAD_THREADS_MAX} (%ld)",
		 made, max);
	pc_note("creation %ld of %ld failed", at + 1, max + 1);
	pc_check(first == EAGAIN, "the failing pthread_create returned %s, expected EAGAIN", pc_errname(first));
	pc_pass();
}
