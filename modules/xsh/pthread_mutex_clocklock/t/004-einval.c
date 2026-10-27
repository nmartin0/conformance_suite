/*
 * A bad nanosecond count or clock makes a blocking timed lock fail with EINVAL.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_mutex_clocklock.004
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

static struct timespec ts_ahead(clockid_t c, long ms)
{
	struct timespec ts;

	NEED(clock_gettime(c, &ts) == 0, "clock_gettime failed");
	ts.tv_sec += ms / 1000;
	ts.tv_nsec += (ms % 1000) * 1000000L;
	if (ts.tv_nsec >= 1000000000L) {
		ts.tv_sec++;
		ts.tv_nsec -= 1000000000L;
	}
	return ts;
}

/* A timed lock made in a helper thread. */
struct call {
	pthread_mutex_t *m;
	int use_clock;		/* 0 pthread_mutex_timedlock, 1 pthread_mutex_clocklock */
	clockid_t clk;		/* the clock of the time, and the clock the end is read on */
	struct timespec abs, end;
	atomic_int done;
	int r;
};

static void *call_main(void *p)
{
	struct call *c = p;

	if (c->use_clock)
		c->r = pthread_mutex_clocklock(c->m, c->clk, &c->abs);
	else
		c->r = pthread_mutex_timedlock(c->m, &c->abs);
	(void)clock_gettime(c->clk, &c->end);
	if (c->r == 0 || c->r == EOWNERDEAD)
		(void)pthread_mutex_unlock(c->m);
	atomic_store(&c->done, 1);
	return NULL;
}

/* Runs the call in a thread; a call still blocked after 10 s fails the test. */
static pthread_t call_start(struct call *c)
{
	pthread_t t;

	atomic_init(&c->done, 0);
	c->r = -1;
	NEED(pthread_create(&t, NULL, call_main, c) == 0, "pthread_create failed");
	return t;
}

static void call_finish(struct call *c, pthread_t t, long limit_ms, const char *what)
{
	long w;

	for (w = 0; w < limit_ms && !atomic_load(&c->done); w += 5)
		sleep_ms(5);
	pc_check(atomic_load(&c->done), "%s did not return within %ld ms", what, limit_ms);
	NEED(pthread_join(t, NULL) == 0, "pthread_join failed");
}


int main(void)
{
	static pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
	static const long candidates[] = { 0x7ffffff0L, 0x12345678L, 4096L, 1000L };
	static const long badns[2] = { -1L, 1000000000L };
	static const clockid_t clks[2] = { CLOCK_REALTIME, CLOCK_MONOTONIC };
	static const char *const cnames[2] = { "CLOCK_REALTIME", "CLOCK_MONOTONIC" };
	struct call c;
	struct timespec ts;
	clockid_t bad = 0;
	pthread_t t;
	unsigned k;
	int i, j, have_bad = 0;

	pc_begin("XSH.pthread_mutex_clocklock.004");
	for (k = 0; k < sizeof candidates / sizeof candidates[0]; k++) {
		bad = (clockid_t)candidates[k];
		if (clock_gettime(bad, &ts) == -1 && errno == EINVAL) {
			have_bad = 1;
			break;
		}
	}
	NEED(pthread_mutex_lock(&m) == 0, "pthread_mutex_lock failed");
	pc_test();
	for (i = 0; i < 2; i++) {
		/* the call would block: the mutex is held */
		c.m = &m;
		c.use_clock = 0;
		c.clk = CLOCK_REALTIME;
		c.abs = ts_ahead(CLOCK_REALTIME, 3000);
		c.abs.tv_nsec = badns[i];
		t = call_start(&c);
		call_finish(&c, t, 10000, "pthread_mutex_timedlock()");
		pc_check(c.r == EINVAL, "pthread_mutex_timedlock() with tv_nsec %ld returned %s, expected EINVAL", badns[i], rn(c.r));
		for (j = 0; j < 2; j++) {
			c.use_clock = 1;
			c.clk = clks[j];
			c.abs = ts_ahead(clks[j], 3000);
			c.abs.tv_nsec = badns[i];
			t = call_start(&c);
			call_finish(&c, t, 10000, "pthread_mutex_clocklock()");
			pc_check(c.r == EINVAL, "pthread_mutex_clocklock(%s) with tv_nsec %ld returned %s, expected EINVAL", cnames[j],
			    badns[i], rn(c.r));
		}
	}
	if (have_bad) {
		c.use_clock = 1;
		c.clk = bad;
		c.abs = ts_ahead(CLOCK_REALTIME, 3000);
		t = call_start(&c);
		call_finish(&c, t, 10000, "pthread_mutex_clocklock() with an unknown clock");
		pc_check(c.r == EINVAL, "pthread_mutex_clocklock() with a clock clock_gettime() rejects returned %s, expected EINVAL",
		    rn(c.r));
	}
	(void)pthread_mutex_unlock(&m);
	pc_pass();
}
