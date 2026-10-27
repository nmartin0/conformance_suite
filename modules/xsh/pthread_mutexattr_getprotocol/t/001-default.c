/*
 * The default mutex protocol is PTHREAD_PRIO_NONE.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_mutexattr_getprotocol.001
 */
#include "pc.h"
#include <errno.h>
#include <pthread.h>
#include <unistd.h>

#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

#if (defined(_POSIX_THREAD_PRIO_INHERIT) && _POSIX_THREAD_PRIO_INHERIT >= 0) || \
    (defined(_POSIX_THREAD_PRIO_PROTECT) && _POSIX_THREAD_PRIO_PROTECT >= 0) || \
    (defined(_POSIX_THREAD_ROBUST_PRIO_INHERIT) && _POSIX_THREAD_ROBUST_PRIO_INHERIT >= 0) || \
    (defined(_POSIX_THREAD_ROBUST_PRIO_PROTECT) && _POSIX_THREAD_ROBUST_PRIO_PROTECT >= 0)
int main(void)
{
	pthread_mutexattr_t a;
	int p = -12345, r;

	pc_begin("XSH.pthread_mutexattr_getprotocol.001");
	NEED(pthread_mutexattr_init(&a) == 0, "pthread_mutexattr_init failed");
	pc_test();
	r = pthread_mutexattr_getprotocol(&a, &p);
	pc_check(r == 0, "pthread_mutexattr_getprotocol() returned %s", r == 0 ? "0" : pc_errname(r));
	pc_check(p == PTHREAD_PRIO_NONE, "a new attributes object has protocol %d, expected PTHREAD_PRIO_NONE (%d)", p,
	    (int)PTHREAD_PRIO_NONE);
	(void)pthread_mutexattr_destroy(&a);
	pc_pass();
}
#else
int main(void)
{
	pc_begin("XSH.pthread_mutexattr_getprotocol.001");
	pc_unsupported("no mutex protocol option is defined");
}
#endif
