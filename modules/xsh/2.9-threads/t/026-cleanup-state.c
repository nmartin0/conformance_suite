/*
 * While cleanup handlers and thread-specific data destructors run, at
 * thread exit or after a cancellation, cancelability is disabled and
 * deferred.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.9-threads.026
 *
 * Thread A sets cancelability asynchronous and calls pthread_exit(); thread
 * B is canceled while blocked in read(). Each has a cleanup handler and a
 * key whose destructor runs. In both, pthread_setcancelstate(DISABLE) and
 * pthread_setcanceltype(DEFERRED) report as the previous values the state
 * DISABLE and the type DEFERRED, in the handler and in the destructor.
 */
#include "pcth.h"

static pthread_key_t key_a, key_b;
static int pfd[2];

/* flags: base+0 handler state, +1 handler type, +2 destructor state, +3 destructor type (0 = not run) */
static void look(int base, int is_dtor)
{
	int s = -1, t = -1;

	(void)pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &s);
	(void)pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, &t);
	flag_set(base + (is_dtor ? 2 : 0), s == PTHREAD_CANCEL_DISABLE ? 1 : 2);
	flag_set(base + (is_dtor ? 3 : 1), t == PTHREAD_CANCEL_DEFERRED ? 1 : 2);
}

static void handler_a(void *p) { (void)p; look(0, 0); }
static void handler_b(void *p) { (void)p; look(4, 0); }

static void dtor_a(void *p) { (void)p; look(0, 1); }
static void dtor_b(void *p) { (void)p; look(4, 1); }

static void *thread_a(void *p)
{
	int old;

	(void)p;
	(void)pthread_setspecific(key_a, &old);
	pthread_cleanup_push(handler_a, NULL);
	(void)pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, &old);
	pthread_exit(NULL);
	pthread_cleanup_pop(0);
	return NULL;
}

static void *thread_b(void *p)
{
	char b;
	int old;

	(void)p;
	(void)pthread_setspecific(key_b, &old);
	pthread_cleanup_push(handler_b, NULL);
	flag_set(8, 1);
	if (read(pfd[0], &b, 1) < 0)
		flag_set(9, 1);
	pthread_cleanup_pop(0);
	return NULL;
}

int main(void)
{
	pthread_t a, b;
	void *st;
	int i;

	pc_begin("XSH.2.9-threads.026");
	guard(60);
	NEED(pipe(pfd) == 0, "pipe failed");
	NEED(pthread_key_create(&key_a, dtor_a) == 0, "pthread_key_create failed");
	NEED(pthread_key_create(&key_b, dtor_b) == 0, "pthread_key_create failed");
	pc_test();
	a = start(thread_a, NULL);
	NEED(join_within(a, 10000, &st), "thread A did not end");
	b = start(thread_b, NULL);
	NEED(wait_flag(8, 1, 10000), "thread B did not start");
	sleep_ms(100);
	NEED(pthread_cancel(b) == 0, "pthread_cancel failed");
	NEED(join_within(b, 10000, &st), "thread B did not end after the request");
	pc_check(st == PTHREAD_CANCELED, "thread B did not end canceled");
	pc_check(flag_get(9) == 0, "read() returned in thread B");
	for (i = 0; i < 8; i += 4) {
		const char *who = i == 0 ? "after pthread_exit()" : "after a cancellation";

		pc_check(flag_get(i) != 0, "%s: the cleanup handler did not run", who);
		pc_check(flag_get(i) == 1, "%s: the cancelability state seen in the cleanup handler was not DISABLE", who);
		pc_check(flag_get(i + 1) == 1, "%s: the cancelability type seen in the cleanup handler was not DEFERRED", who);
		pc_check(flag_get(i + 2) != 0, "%s: the key's destructor did not run", who);
		pc_check(flag_get(i + 2) == 1, "%s: the cancelability state seen in the destructor was not DISABLE", who);
		pc_check(flag_get(i + 3) == 1, "%s: the cancelability type seen in the destructor was not DEFERRED", who);
	}
	pc_pass();
}
