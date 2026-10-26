/*
 * A new thread does not inherit its creator's thread-local locale.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_create.007
 * MUTANT    pthread_create.locale_inherited
 * VARIANT   pthread_create.runs_late
 */
#include "pc.h"
#include <errno.h>
#include <locale.h>
#include <pthread.h>
#include <time.h>

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

static locale_t mine;
static int is_global = -1, is_mine = -1;

static void *start(void *arg)
{
	locale_t cur = uselocale((locale_t)0);

	(void)arg;
	(void)pthread_mutex_lock(&mx);
	is_global = cur == LC_GLOBAL_LOCALE;
	is_mine = cur == mine;
	(void)pthread_mutex_unlock(&mx);
	return NULL;
}

int main(void)
{
	pthread_t t;
	int r, g, m;

	pc_begin("XSH.pthread_create.007");
	mine = newlocale(LC_ALL_MASK, "POSIX", (locale_t)0);
	pc_check(mine != (locale_t)0, "newlocale failed: %s", pc_errname(errno));
	pc_check(uselocale(mine) != (locale_t)0, "uselocale failed: %s", pc_errname(errno));
	pc_check(uselocale((locale_t)0) == mine, "uselocale did not install the thread-local locale");
	pc_test();
	r = pthread_create(&t, NULL, start, NULL);
	if (r != 0)
		pc_unresolved("pthread_create failed: %s", pc_errname(r));
	if (pthread_join(t, NULL) != 0)
		pc_unresolved("pthread_join failed");
	(void)pthread_mutex_lock(&mx);
	g = is_global;
	m = is_mine;
	(void)pthread_mutex_unlock(&mx);
	pc_check(m == 0, "the new thread uses its creator's thread-local locale");
	pc_check(g == 1, "the new thread's uselocale((locale_t)0) is not LC_GLOBAL_LOCALE: it has a thread-local locale");
	pc_pass();
}
