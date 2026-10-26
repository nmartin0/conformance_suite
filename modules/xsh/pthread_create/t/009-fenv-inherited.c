/*
 * A new thread starts with its creator's floating-point environment.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_create.009
 * MUTANT    pthread_create.fenv_reset
 * VARIANT   pthread_create.runs_late
 */
#include "pc.h"
#include <errno.h>
#include <fenv.h>
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

static int round_seen = -12345, flag_seen = -1;

static void *start(void *arg)
{
	int rd = fegetround(), fl = -1;

	(void)arg;
#ifdef FE_DIVBYZERO
	fl = fetestexcept(FE_DIVBYZERO) != 0;
#endif
	(void)pthread_mutex_lock(&mx);
	round_seen = rd;
	flag_seen = fl;
	(void)pthread_mutex_unlock(&mx);
	return NULL;
}

int main(void)
{
	static const int modes[] = {
#ifdef FE_UPWARD
		FE_UPWARD,
#endif
#ifdef FE_DOWNWARD
		FE_DOWNWARD,
#endif
#ifdef FE_TOWARDZERO
		FE_TOWARDZERO,
#endif
		-1
	};
	int i, want = -1, have_flag = 0, r, rd, fl, initial;
	pthread_t t;

	pc_begin("XSH.pthread_create.009");
	initial = fegetround();
	for (i = 0; modes[i] != -1; i++)
		if (modes[i] != initial && fesetround(modes[i]) == 0 && fegetround() == modes[i]) {
			want = modes[i];
			break;
		}
	if (want == -1)
		pc_notinuse("no rounding direction other than the current one can be set");
#ifdef FE_DIVBYZERO
	(void)feclearexcept(FE_ALL_EXCEPT);
	have_flag = feraiseexcept(FE_DIVBYZERO) == 0 && fetestexcept(FE_DIVBYZERO) != 0;
#endif
	pc_test();
	r = pthread_create(&t, NULL, start, NULL);
	if (r != 0)
		pc_unresolved("pthread_create failed: %s", pc_errname(r));
	if (pthread_join(t, NULL) != 0)
		pc_unresolved("pthread_join failed");
	(void)pthread_mutex_lock(&mx);
	rd = round_seen;
	fl = flag_seen;
	(void)pthread_mutex_unlock(&mx);
	pc_check(rd == want, "the new thread's rounding direction is %d, its creator's %d", rd, want);
	if (have_flag)
		pc_check(fl == 1, "FE_DIVBYZERO, raised in the creator, is clear in the new thread");
	pc_pass();
}
