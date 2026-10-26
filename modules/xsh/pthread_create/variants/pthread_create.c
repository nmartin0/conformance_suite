/*
 * pthread_create.c - conforming variants of pthread_create(), for the
 * variant checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; see core/pcvariant.h. Each variant says why the
 * standard allows it.
 *
 * "runs_late": the new thread starts its start routine 50 ms after it is
 * created. "runs_first": pthread_create() returns only once the new thread
 * has begun. XSH pthread_create() sets no order between the creator going
 * on and the new thread starting; scheduling decides (XSH 2.9.4).
 *
 * "late_store": the ID reaches *thread only after the new thread has
 * begun and run for a while. pthread_create() APPLICATION USAGE says
 * nothing requires the ID to be there before the thread starts; it must
 * be there when pthread_create() returns.
 *
 * "altstack_fresh": every new thread is given an alternate signal stack
 * of its own. XSH pthread_create() forbids inheriting the creator's; it
 * does not forbid the implementation from providing another one.
 *
 * "threads_max_64": {PTHREAD_THREADS_MAX} is 64 (the least XBD <limits.h>
 * allows, {_POSIX_THREAD_THREADS_MAX}), reported by sysconf(), and a
 * creation that would make the process's threads, the initial one
 * included, more than 64 fails with EAGAIN, as XSH pthread_create()
 * ERRORS requires.
 */
#define _GNU_SOURCE 1
#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "pcvariant.h"

static void nap_ms(long ms)
{
	struct timespec ts = { ms / 1000, (ms % 1000) * 1000000L };

	while (nanosleep(&ts, &ts) == -1 && errno == EINTR)
		;
}

static pthread_mutex_t lk = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cv = PTHREAD_COND_INITIALIZER;
static int threads = 1;	/* the initial thread */
static pthread_key_t count_key;
static pthread_once_t count_once = PTHREAD_ONCE_INIT;

static void uncount(void *p)
{
	(void)p;
	(void)pthread_mutex_lock(&lk);
	threads--;
	(void)pthread_mutex_unlock(&lk);
}

static void make_count_key(void)
{
	(void)pthread_key_create(&count_key, uncount);
}

struct start {
	void *(*fn)(void *);
	void *arg;
	int begun;
	int counted;
	const char *v;
};

static void begin(struct start *w)
{
	(void)pthread_mutex_lock(&lk);
	w->begun = 1;
	(void)pthread_cond_broadcast(&cv);
	(void)pthread_mutex_unlock(&lk);
	/* w belongs to the creator from here on */
}

static void *trampoline(void *p)
{
	struct start *w = p;
	void *(*fn)(void *) = w->fn;
	void *arg = w->arg;
	const char *v = w->v;

	if (w->counted)
		(void)pthread_setspecific(count_key, (void *)1);
	if (IS("altstack_fresh")) {
		stack_t ss;
		ss.ss_size = (size_t)SIGSTKSZ * 2;
		ss.ss_sp = malloc(ss.ss_size);
		ss.ss_flags = 0;
		if (ss.ss_sp != NULL)
			(void)sigaltstack(&ss, NULL);
	}
	if (!IS("runs_first"))
		begin(w);
	if (IS("runs_late"))
		nap_ms(50);
	if (IS("runs_first"))
		begin(w);	/* the creator returns only now */
	return fn(arg);
}

long sysconf(int name)
{
	const char *v = variant_of("pthread_create.");
	REAL(sysconf);

	if (IS("threads_max_64") && name == _SC_THREAD_THREADS_MAX)
		return 64;
	return real_sysconf(name);
}

int pthread_create(pthread_t *restrict t, const pthread_attr_t *restrict attr,
		   void *(*fn)(void *), void *restrict arg)
{
	const char *v = variant_of("pthread_create.");
	struct start *w;
	pthread_t got;
	int r;
	REAL(pthread_create);

	if (v == NULL || !(IS("runs_late") || IS("runs_first") || IS("late_store")
	    || IS("altstack_fresh") || IS("threads_max_64")))
		return real_pthread_create(t, attr, fn, arg);
	w = calloc(1, sizeof *w);
	if (w == NULL)
		return EAGAIN;
	w->fn = fn;
	w->arg = arg;
	w->v = v;
	if (IS("threads_max_64")) {
		(void)pthread_once(&count_once, make_count_key);
		(void)pthread_mutex_lock(&lk);
		if (threads + 1 > 64) {
			(void)pthread_mutex_unlock(&lk);
			free(w);
			return EAGAIN;
		}
		threads++;
		(void)pthread_mutex_unlock(&lk);
		w->counted = 1;
	}
	if (IS("late_store"))
		memset(t, 0xa5, sizeof *t);
	r = real_pthread_create(&got, attr, trampoline, w);
	if (r != 0) {
		if (w->counted)
			uncount(NULL);
		free(w);
		return r;
	}
	/* wait for the trampoline to be done with w */
	(void)pthread_mutex_lock(&lk);
	while (!w->begun)
		(void)pthread_cond_wait(&cv, &lk);
	(void)pthread_mutex_unlock(&lk);
	free(w);
	if (IS("late_store"))
		nap_ms(40);
	*t = got;
	return 0;
}
