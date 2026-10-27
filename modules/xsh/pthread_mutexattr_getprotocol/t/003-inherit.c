/*
 * PTHREAD_PRIO_INHERIT can be set and is reported.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_mutexattr_getprotocol.003
 */
#include "pc.h"
#include <errno.h>
#include <pthread.h>
#include <unistd.h>

#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

#if (defined(_POSIX_THREAD_PRIO_INHERIT) && _POSIX_THREAD_PRIO_INHERIT >= 0) || \
    (defined(_POSIX_THREAD_ROBUST_PRIO_INHERIT) && _POSIX_THREAD_ROBUST_PRIO_INHERIT >= 0)
static const char *rn(int r)
{
	return r == 0 ? "0" : pc_errname(r);
}

int main(void)
{
	pthread_mutexattr_t a;
	int p = -12345, r;

	pc_begin("XSH.pthread_mutexattr_getprotocol.003");
	NEED(pthread_mutexattr_init(&a) == 0, "pthread_mutexattr_init failed");
	pc_test();
	r = pthread_mutexattr_setprotocol(&a, PTHREAD_PRIO_INHERIT);
	if (r == EPERM)
		pc_unresolved("pthread_mutexattr_setprotocol(PTHREAD_PRIO_INHERIT) needs a privilege the caller lacks");
	pc_check(r == 0, "pthread_mutexattr_setprotocol(PTHREAD_PRIO_INHERIT) returned %s", rn(r));
	r = pthread_mutexattr_getprotocol(&a, &p);
	pc_check(r == 0, "pthread_mutexattr_getprotocol() returned %s", rn(r));
	pc_check(p == PTHREAD_PRIO_INHERIT, "after setting PTHREAD_PRIO_INHERIT (%d), getprotocol stored %d",
	    (int)PTHREAD_PRIO_INHERIT, p);
	(void)pthread_mutexattr_destroy(&a);
	pc_pass();
}
#else
int main(void)
{
	pc_begin("XSH.pthread_mutexattr_getprotocol.003");
	pc_unsupported("no priority inheritance option is defined");
}
#endif
