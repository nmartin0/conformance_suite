/*
 * pthread_mutexattr_init() gives the protocol attribute its default.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_mutexattr_init.003
 */
#include "pc.h"
#include <errno.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

#if (defined(_POSIX_THREAD_PRIO_INHERIT) && _POSIX_THREAD_PRIO_INHERIT >= 0) || \
    (defined(_POSIX_THREAD_PRIO_PROTECT) && _POSIX_THREAD_PRIO_PROTECT >= 0) || \
    (defined(_POSIX_THREAD_ROBUST_PRIO_INHERIT) && _POSIX_THREAD_ROBUST_PRIO_INHERIT >= 0) || \
    (defined(_POSIX_THREAD_ROBUST_PRIO_PROTECT) && _POSIX_THREAD_ROBUST_PRIO_PROTECT >= 0)
static const char *rn(int r)
{
	return r == 0 ? "0" : pc_errname(r);
}

static void check_default(pthread_mutexattr_t *a, const char *when)
{
	int p = -12345, r;

	r = pthread_mutexattr_getprotocol(a, &p);
	pc_check(r == 0, "%s: pthread_mutexattr_getprotocol() returned %s", when, rn(r));
	pc_check(p == PTHREAD_PRIO_NONE, "%s: protocol is %d, expected PTHREAD_PRIO_NONE (%d)", when, p,
	    (int)PTHREAD_PRIO_NONE);
}

int main(void)
{
	static const unsigned char fill[] = { 0x00, 0xff, 0xa5 };
	pthread_mutexattr_t a;
	unsigned i;
	int r;

	pc_begin("XSH.pthread_mutexattr_init.003");
	pc_test();
	for (i = 0; i < sizeof fill; i++) {
		memset(&a, fill[i], sizeof a);
		r = pthread_mutexattr_init(&a);
		pc_check(r == 0, "pthread_mutexattr_init() on storage filled with %#x returned %s", fill[i], rn(r));
		check_default(&a, "after init on filled storage");
		(void)pthread_mutexattr_destroy(&a);
	}
	r = pthread_mutexattr_init(&a);
	pc_check(r == 0, "pthread_mutexattr_init() returned %s", rn(r));
#if defined(_POSIX_THREAD_PRIO_INHERIT) && _POSIX_THREAD_PRIO_INHERIT >= 0
	r = pthread_mutexattr_setprotocol(&a, PTHREAD_PRIO_INHERIT);
#elif defined(_POSIX_THREAD_PRIO_PROTECT) && _POSIX_THREAD_PRIO_PROTECT >= 0
	r = pthread_mutexattr_setprotocol(&a, PTHREAD_PRIO_PROTECT);
#else
	r = 0;
#endif
	if (r != 0)
		pc_unresolved("pthread_mutexattr_setprotocol() of a non-default protocol returned %s", rn(r));
	r = pthread_mutexattr_destroy(&a);
	pc_check(r == 0, "pthread_mutexattr_destroy() returned %s", rn(r));
	r = pthread_mutexattr_init(&a);
	pc_check(r == 0, "pthread_mutexattr_init() after the destroy returned %s", rn(r));
	check_default(&a, "after destroy and init of a changed object");
	(void)pthread_mutexattr_destroy(&a);
	pc_pass();
}
#else
int main(void)
{
	pc_begin("XSH.pthread_mutexattr_init.003");
	pc_unsupported("no mutex protocol option is defined");
}
#endif
