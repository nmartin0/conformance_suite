/*
 * Condition waits and joins listed as cancellation points act on a pending
 * cancellation request before they return.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.9-threads.015
 *
 * For each of pthread_cond_wait(), pthread_cond_timedwait(),
 * pthread_cond_clockwait(), cnd_wait(), cnd_timedwait(), pthread_join()
 * and thrd_join(): see XSH.2.9-threads.012. Each condition wait is made
 * with its mutex locked by the calling thread (taking the lock is not a
 * cancellation point) and, where timed, a limit 30 s ahead.
 */
#include "pccp.h"
#include <unistd.h>
#ifdef __STDC_NO_THREADS__
int main(void)
{
	pc_begin("XSH.2.9-threads.015");
#ifdef _POSIX_SUBPROFILE
	pc_notinuse("<threads.h> is not provided (a subprofile implementation may define __STDC_NO_THREADS__)");
#else
	pc_test();
	pc_fail("__STDC_NO_THREADS__ is defined, but <threads.h> is required unless _POSIX_SUBPROFILE is defined");
#endif
}
#else
#include <threads.h>

static volatile long sink;
static pthread_mutex_t pm[3] = { PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER };
static pthread_cond_t pcv[3] = { PTHREAD_COND_INITIALIZER, PTHREAD_COND_INITIALIZER, PTHREAD_COND_INITIALIZER };
static mtx_t cm[2];
static cnd_t ccv[2];
static pthread_t jt;
static thrd_t tt;

static void *pt_body(void *a)
{
	(void)a;
	while (!flag_get(10))
		sleep_ms(1);
	return NULL;
}

static int tt_body(void *a)
{
	(void)a;
	while (!flag_get(10))
		sleep_ms(1);
	return 0;
}

static void s_pjoin(void)
{
	flag_set(10, 0);
	jt = start(pt_body, NULL);
}

static void d_pjoin(void)
{
	flag_set(10, 1);
	(void)pthread_join(jt, NULL);
}

static void s_tjoin(void)
{
	flag_set(10, 0);
	NEED(thrd_create(&tt, tt_body, NULL) == thrd_success, "thrd_create failed");
}

static void d_tjoin(void)
{
	flag_set(10, 1);
	(void)thrd_join(tt, NULL);
}

static void c_cond_wait(void) { (void)pthread_mutex_lock(&pm[0]); sink = pthread_cond_wait(&pcv[0], &pm[0]); }
static void c_cond_timedwait(void)
{
	struct timespec ts = ts_after(CLOCK_REALTIME, 30000);

	(void)pthread_mutex_lock(&pm[1]);
	sink = pthread_cond_timedwait(&pcv[1], &pm[1], &ts);
}
static void c_cond_clockwait(void)
{
	struct timespec ts = ts_after(CLOCK_MONOTONIC, 30000);

	(void)pthread_mutex_lock(&pm[2]);
	sink = pthread_cond_clockwait(&pcv[2], &pm[2], CLOCK_MONOTONIC, &ts);
}
static void c_cnd_wait(void) { (void)mtx_lock(&cm[0]); sink = cnd_wait(&ccv[0], &cm[0]); }
static void c_cnd_timedwait(void)
{
	struct timespec ts;

	(void)timespec_get(&ts, TIME_UTC);
	ts.tv_sec += 30;
	(void)mtx_lock(&cm[1]);
	sink = cnd_timedwait(&ccv[1], &cm[1], &ts);
}
static void c_pthread_join(void) { sink = pthread_join(jt, NULL); }
static void c_thrd_join(void) { sink = thrd_join(tt, NULL); }

static const struct cpcase cases[] = {
	{ "pthread_cond_wait", NULL, c_cond_wait, NULL },
	{ "pthread_cond_timedwait", NULL, c_cond_timedwait, NULL },
	{ "pthread_cond_clockwait", NULL, c_cond_clockwait, NULL },
	{ "cnd_wait", NULL, c_cnd_wait, NULL },
	{ "cnd_timedwait", NULL, c_cnd_timedwait, NULL },
	{ "pthread_join", s_pjoin, c_pthread_join, d_pjoin },
	{ "thrd_join", s_tjoin, c_thrd_join, d_tjoin },
};

int main(void)
{
	char bad[512];
	int n, i;

	pc_begin("XSH.2.9-threads.015");
	guard(250);
	for (i = 0; i < 2; i++) {
		NEED(mtx_init(&cm[i], mtx_plain) == thrd_success, "mtx_init failed");
		NEED(cnd_init(&ccv[i]) == thrd_success, "cnd_init failed");
	}
	pc_test();
	n = cp_run(cases, (int)(sizeof cases / sizeof cases[0]), CP_PENDING, bad, sizeof bad);
	pc_check(n == 0, "with a cancellation request pending and cancelability enabled the thread was not canceled in: %s", bad);
	pc_pass();
}
#endif
