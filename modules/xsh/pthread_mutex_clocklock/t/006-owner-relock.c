/*
 * A timed lock on a mutex the caller owns: EDEADLK (error-checking) or recursion.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_mutex_clocklock.006
 */
#include "pc.h"
#include <errno.h>
#include <pthread.h>
#include <time.h>

#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

static const char *rn(int r)
{
	return r == 0 ? "0" : pc_errname(r);
}

static void make(pthread_mutex_t *m, int type)
{
	pthread_mutexattr_t a;

	NEED(pthread_mutexattr_init(&a) == 0, "pthread_mutexattr_init failed");
	NEED(pthread_mutexattr_settype(&a, type) == 0, "pthread_mutexattr_settype failed");
	NEED(pthread_mutex_init(m, &a) == 0, "pthread_mutex_init failed");
	(void)pthread_mutexattr_destroy(&a);
}

int main(void)
{
	pthread_mutex_t chk, rec;
	struct timespec abs;
	int r;

	pc_begin("XSH.pthread_mutex_clocklock.006");
	make(&chk, PTHREAD_MUTEX_ERRORCHECK);
	make(&rec, PTHREAD_MUTEX_RECURSIVE);
	NEED(clock_gettime(CLOCK_REALTIME, &abs) == 0, "clock_gettime failed");
	abs.tv_sec += 5;
	NEED(pthread_mutex_lock(&chk) == 0, "pthread_mutex_lock failed");
	NEED(pthread_mutex_lock(&rec) == 0, "pthread_mutex_lock failed");
	pc_test();
	r = pthread_mutex_timedlock(&chk, &abs);
	pc_check(r == EDEADLK, "pthread_mutex_timedlock() of an owned error-checking mutex returned %s, expected EDEADLK", rn(r));
	r = pthread_mutex_clocklock(&chk, CLOCK_REALTIME, &abs);
	pc_check(r == EDEADLK, "pthread_mutex_clocklock() of an owned error-checking mutex returned %s, expected EDEADLK", rn(r));
	r = pthread_mutex_unlock(&chk);
	pc_check(r == 0, "unlock of the error-checking mutex returned %s", rn(r));
	r = pthread_mutex_timedlock(&rec, &abs);
	pc_check(r == 0, "pthread_mutex_timedlock() of an owned recursive mutex returned %s", rn(r));
	r = pthread_mutex_clocklock(&rec, CLOCK_REALTIME, &abs);
	pc_check(r == 0, "pthread_mutex_clocklock() of an owned recursive mutex returned %s", rn(r));
	r = pthread_mutex_unlock(&rec);
	pc_check(r == 0, "first unlock of the recursive mutex returned %s", rn(r));
	r = pthread_mutex_unlock(&rec);
	pc_check(r == 0, "second unlock of the recursive mutex returned %s", rn(r));
	r = pthread_mutex_unlock(&rec);
	pc_check(r == 0, "third unlock of the recursive mutex returned %s", rn(r));
	r = pthread_mutex_unlock(&rec);
	pc_check(r == EPERM, "fourth unlock of the recursive mutex (no longer owned) returned %s, expected EPERM", rn(r));
	pc_pass();
}
