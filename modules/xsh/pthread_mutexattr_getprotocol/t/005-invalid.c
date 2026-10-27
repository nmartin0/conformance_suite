/*
 * pthread_mutexattr_setprotocol() fails for a value that is no protocol constant.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_mutexattr_getprotocol.005
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
static int is_protocol(int v)
{
	if (v == PTHREAD_PRIO_NONE)
		return 1;
#if (defined(_POSIX_THREAD_PRIO_INHERIT) && _POSIX_THREAD_PRIO_INHERIT >= 0) || \
    (defined(_POSIX_THREAD_ROBUST_PRIO_INHERIT) && _POSIX_THREAD_ROBUST_PRIO_INHERIT >= 0)
	if (v == PTHREAD_PRIO_INHERIT)
		return 1;
#endif
#if (defined(_POSIX_THREAD_PRIO_PROTECT) && _POSIX_THREAD_PRIO_PROTECT >= 0) || \
    (defined(_POSIX_THREAD_ROBUST_PRIO_PROTECT) && _POSIX_THREAD_ROBUST_PRIO_PROTECT >= 0)
	if (v == PTHREAD_PRIO_PROTECT)
		return 1;
#endif
	return 0;
}

static const char *rn(int r)
{
	return r == 0 ? "0" : pc_errname(r);
}

int main(void)
{
	static const int bad[] = { 0x7ffffff0, -0x7ffffff0 };
	pthread_mutexattr_t a;
	unsigned i;
	int r;

	pc_begin("XSH.pthread_mutexattr_getprotocol.005");
	NEED(pthread_mutexattr_init(&a) == 0, "pthread_mutexattr_init failed");
	for (i = 0; i < 2; i++)
		NEED(!is_protocol(bad[i]), "%#x is the value of a standard protocol here", (unsigned)bad[i]);
	pc_test();
	for (i = 0; i < 2; i++) {
		r = pthread_mutexattr_setprotocol(&a, bad[i]);
		pc_check(r == ENOTSUP || r == EINVAL, "pthread_mutexattr_setprotocol(%d) returned %s, expected ENOTSUP or EINVAL",
		    bad[i], rn(r));
	}
	(void)pthread_mutexattr_destroy(&a);
	pc_pass();
}
#else
int main(void)
{
	pc_begin("XSH.pthread_mutexattr_getprotocol.005");
	pc_unsupported("no mutex protocol option is defined");
}
#endif
