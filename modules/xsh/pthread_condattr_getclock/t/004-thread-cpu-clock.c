/*
 * pthread_condattr_setclock() refuses the CPU-time clock of a thread.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_condattr_getclock.004
 */
#include "pc.h"
#include <errno.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

#if defined(_POSIX_THREAD_CPUTIME) && _POSIX_THREAD_CPUTIME >= 0 && defined(CLOCK_THREAD_CPUTIME_ID)
int main(void)
{
	pthread_condattr_t a;
	clockid_t id;
	int r;

	pc_begin("XSH.pthread_condattr_getclock.004");
	NEED(pthread_condattr_init(&a) == 0, "pthread_condattr_init failed");
	NEED(pthread_getcpuclockid(pthread_self(), &id) == 0, "pthread_getcpuclockid failed");
	pc_test();
	r = pthread_condattr_setclock(&a, CLOCK_THREAD_CPUTIME_ID);
	pc_check(r > 0, "pthread_condattr_setclock(CLOCK_THREAD_CPUTIME_ID) returned %d, expected an error number (a positive value)", r);
	r = pthread_condattr_setclock(&a, id);
	pc_check(r > 0, "pthread_condattr_setclock() with the thread CPU-time clock from pthread_getcpuclockid() returned %d, expected an error number (a positive value)", r);
	(void)pthread_condattr_destroy(&a);
	pc_pass();
}
#else
int main(void)
{
	pc_begin("XSH.pthread_condattr_getclock.004");
	pc_unsupported("_POSIX_THREAD_CPUTIME is not defined");
}
#endif
