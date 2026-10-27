/*
 * pthread_rwlockattr_getpshared() reports what pthread_rwlockattr_setpshared() set.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_rwlockattr_getpshared.002
 */
#include "pc.h"
#include <errno.h>
#include <pthread.h>
#include <unistd.h>

#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

#if defined(_POSIX_THREAD_PROCESS_SHARED) && _POSIX_THREAD_PROCESS_SHARED >= 0
static const char *rn(int r)
{
	return r == 0 ? "0" : pc_errname(r);
}

int main(void)
{
	static const int vals[] = { PTHREAD_PROCESS_SHARED, PTHREAD_PROCESS_PRIVATE, PTHREAD_PROCESS_SHARED };
	static const char *const names[] = { "SHARED", "PRIVATE", "SHARED" };
	pthread_rwlockattr_t a;
	unsigned i;
	int p, r;

	pc_begin("XSH.pthread_rwlockattr_getpshared.002");
	NEED(pthread_rwlockattr_init(&a) == 0, "pthread_rwlockattr_init failed");
	pc_test();
	for (i = 0; i < 3; i++) {
		r = pthread_rwlockattr_setpshared(&a, vals[i]);
		pc_check(r == 0, "pthread_rwlockattr_setpshared(PTHREAD_PROCESS_%s) returned %s", names[i], rn(r));
		p = -12345;
		r = pthread_rwlockattr_getpshared(&a, &p);
		pc_check(r == 0, "pthread_rwlockattr_getpshared() returned %s", rn(r));
		pc_check(p == vals[i], "after setting PTHREAD_PROCESS_%s (%d), getpshared stored %d", names[i], vals[i], p);
	}
	(void)pthread_rwlockattr_destroy(&a);
	pc_pass();
}
#else
int main(void)
{
	pc_begin("XSH.pthread_rwlockattr_getpshared.002");
	pc_unsupported("_POSIX_THREAD_PROCESS_SHARED is not defined");
}
#endif
