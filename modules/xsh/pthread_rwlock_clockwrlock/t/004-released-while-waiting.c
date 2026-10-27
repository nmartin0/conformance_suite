/*
 * A waiting timed write lock gets the lock when it is released.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_rwlock_clockwrlock.004
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

/* A timed write lock made in a helper thread. */
struct call {
	pthread_rwlock_t *l;
	int use_clock;		/* 0 pthread_rwlock_timedwrlock, 1 pthread_rwlock_clockwrlock */
	clockid_t clk;		/* the clock of the time, and the clock the end is read on */
	struct timespec abs, end;
	atomic_int done;
	int r;
};

static void *call_main(void *p)
{
	struct call *c = p;

	if (c->use_clock)
		c->r = pthread_rwlock_clockwrlock(c->l, c->clk, &c->abs);
	else
		c->r = pthread_rwlock_timedwrlock(c->l, &c->abs);
	(void)clock_gettime(c->clk, &c->end);
	if (c->r == 0)
		(void)pthread_rwlock_unlock(c->l);
	atomic_store(&c->done, 1);
	return NULL;
}

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
	pthread_rwlock_t l = PTHREAD_RWLOCK_INITIALIZER;
	struct call c;
	pthread_t t;
	int r;

	pc_begin("XSH.pthread_rwlock_clockwrlock.004");
	NEED(pthread_rwlock_wrlock(&l) == 0, "pthread_rwlock_wrlock failed");
	c.l = &l;
	c.use_clock = 0;
	c.clk = CLOCK_REALTIME;
	c.abs = ts_ahead(CLOCK_REALTIME, 10000);
	pc_test();
	t = call_start(&c);
	sleep_ms(150);
	pc_check(!atomic_load(&c.done), "pthread_rwlock_timedwrlock() returned %s while the write lock was held", rn(c.r));
	r = pthread_rwlock_unlock(&l);
	pc_check(r == 0, "pthread_rwlock_unlock() returned %s", rn(r));
	call_finish(&c, t, 5000, "pthread_rwlock_timedwrlock() after the unlock");
	pc_check(c.r == 0, "pthread_rwlock_timedwrlock() returned %s after the write lock was released, expected 0", rn(c.r));
	pc_pass();
}
