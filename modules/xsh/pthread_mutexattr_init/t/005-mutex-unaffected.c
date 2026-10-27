/*
 * Changing or destroying an attributes object leaves a mutex made from it alone.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_mutexattr_init.005
 */
#include "pc.h"
#include <errno.h>
#include <pthread.h>

#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

static const char *rn(int r)
{
	return r == 0 ? "0" : pc_errname(r);
}

int main(void)
{
	pthread_mutexattr_t a;
	pthread_mutex_t rec, chk;
	int r;

	pc_begin("XSH.pthread_mutexattr_init.005");
	NEED(pthread_mutexattr_init(&a) == 0, "pthread_mutexattr_init failed");
	NEED(pthread_mutexattr_settype(&a, PTHREAD_MUTEX_RECURSIVE) == 0, "settype(RECURSIVE) failed");
	NEED(pthread_mutex_init(&rec, &a) == 0, "pthread_mutex_init(recursive) failed");
	NEED(pthread_mutexattr_settype(&a, PTHREAD_MUTEX_ERRORCHECK) == 0, "settype(ERRORCHECK) failed");
	NEED(pthread_mutex_init(&chk, &a) == 0, "pthread_mutex_init(errorcheck) failed");
	NEED(pthread_mutexattr_settype(&a, PTHREAD_MUTEX_RECURSIVE) == 0, "settype(RECURSIVE) failed");
	pc_test();
	r = pthread_mutexattr_destroy(&a);
	pc_check(r == 0, "pthread_mutexattr_destroy() returned %s", rn(r));

	r = pthread_mutex_lock(&rec);
	pc_check(r == 0, "first lock of the recursive mutex returned %s", rn(r));
	r = pthread_mutex_lock(&rec);
	pc_check(r == 0, "second lock of the recursive mutex (attributes changed and destroyed since) returned %s", rn(r));
	r = pthread_mutex_unlock(&rec);
	pc_check(r == 0, "first unlock of the recursive mutex returned %s", rn(r));
	r = pthread_mutex_unlock(&rec);
	pc_check(r == 0, "second unlock of the recursive mutex returned %s", rn(r));

	r = pthread_mutex_lock(&chk);
	pc_check(r == 0, "lock of the error-checking mutex returned %s", rn(r));
	r = pthread_mutex_lock(&chk);
	pc_check(r == EDEADLK, "second lock of the error-checking mutex (attributes changed and destroyed since) returned %s, expected EDEADLK", rn(r));
	r = pthread_mutex_unlock(&chk);
	pc_check(r == 0, "unlock of the error-checking mutex returned %s", rn(r));
	(void)pthread_mutex_destroy(&rec);
	(void)pthread_mutex_destroy(&chk);
	pc_pass();
}
