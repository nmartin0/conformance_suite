/*
 * pthread_condattr_setclock() refuses the CPU-time clock of the process.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_condattr_getclock.003
 */
#include "pc.h"
#include <errno.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

#if defined(_POSIX_CPUTIME) && _POSIX_CPUTIME >= 0 && defined(CLOCK_PROCESS_CPUTIME_ID)
int main(void)
{
	pthread_condattr_t a;
	clockid_t id;
	int r;

	pc_begin("XSH.pthread_condattr_getclock.003");
	NEED(pthread_condattr_init(&a) == 0, "pthread_condattr_init failed");
	NEED(clock_getcpuclockid(getpid(), &id) == 0, "clock_getcpuclockid failed");
	pc_test();
	r = pthread_condattr_setclock(&a, CLOCK_PROCESS_CPUTIME_ID);
	pc_check(r != 0, "pthread_condattr_setclock(CLOCK_PROCESS_CPUTIME_ID) returned 0, expected a failure");
	r = pthread_condattr_setclock(&a, id);
	pc_check(r != 0, "pthread_condattr_setclock() with the process CPU-time clock from clock_getcpuclockid() returned 0, expected a failure");
	(void)pthread_condattr_destroy(&a);
	pc_pass();
}
#else
int main(void)
{
	pc_begin("XSH.pthread_condattr_getclock.003");
	pc_unsupported("_POSIX_CPUTIME is not defined");
}
#endif
