/*
 * The default process-shared attribute of a mutex is PTHREAD_PROCESS_PRIVATE.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_mutexattr_getpshared.001
 */
#include "pc.h"
#include <errno.h>
#include <pthread.h>
#include <unistd.h>

#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

#if defined(_POSIX_THREAD_PROCESS_SHARED) && _POSIX_THREAD_PROCESS_SHARED >= 0
int main(void)
{
	pthread_mutexattr_t a;
	int p = -12345, r;

	pc_begin("XSH.pthread_mutexattr_getpshared.001");
	NEED(pthread_mutexattr_init(&a) == 0, "pthread_mutexattr_init failed");
	pc_test();
	r = pthread_mutexattr_getpshared(&a, &p);
	pc_check(r == 0, "pthread_mutexattr_getpshared() returned %s", r == 0 ? "0" : pc_errname(r));
	pc_check(p == PTHREAD_PROCESS_PRIVATE, "a new attributes object has process-shared %d, expected PTHREAD_PROCESS_PRIVATE (%d)",
	    p, (int)PTHREAD_PROCESS_PRIVATE);
	(void)pthread_mutexattr_destroy(&a);
	pc_pass();
}
#else
int main(void)
{
	pc_begin("XSH.pthread_mutexattr_getpshared.001");
	pc_unsupported("_POSIX_THREAD_PROCESS_SHARED is not defined");
}
#endif
