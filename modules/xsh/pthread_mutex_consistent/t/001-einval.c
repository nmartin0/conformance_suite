/*
 * pthread_mutex_consistent() fails with EINVAL when there is nothing to make consistent.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_mutex_consistent.001
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
	pthread_mutex_t plain, robust;
	pthread_mutexattr_t a;
	int r;

	pc_begin("XSH.pthread_mutex_consistent.001");
	NEED(pthread_mutex_init(&plain, NULL) == 0, "pthread_mutex_init failed");
	NEED(pthread_mutexattr_init(&a) == 0, "pthread_mutexattr_init failed");
	NEED(pthread_mutexattr_setrobust(&a, PTHREAD_MUTEX_ROBUST) == 0, "pthread_mutexattr_setrobust failed");
	NEED(pthread_mutex_init(&robust, &a) == 0, "pthread_mutex_init(robust) failed");
	(void)pthread_mutexattr_destroy(&a);
	pc_test();
	r = pthread_mutex_consistent(&plain);
	pc_check(r == EINVAL, "pthread_mutex_consistent() of an unlocked non-robust mutex returned %s, expected EINVAL", rn(r));
	NEED(pthread_mutex_lock(&plain) == 0, "pthread_mutex_lock failed");
	r = pthread_mutex_consistent(&plain);
	pc_check(r == EINVAL, "pthread_mutex_consistent() of a locked non-robust mutex returned %s, expected EINVAL", rn(r));
	(void)pthread_mutex_unlock(&plain);
	r = pthread_mutex_consistent(&robust);
	pc_check(r == EINVAL, "pthread_mutex_consistent() of an unlocked consistent robust mutex returned %s, expected EINVAL", rn(r));
	NEED(pthread_mutex_lock(&robust) == 0, "pthread_mutex_lock(robust) failed");
	r = pthread_mutex_consistent(&robust);
	pc_check(r == EINVAL, "pthread_mutex_consistent() of a locked consistent robust mutex returned %s, expected EINVAL", rn(r));
	(void)pthread_mutex_unlock(&robust);
	pc_pass();
}
