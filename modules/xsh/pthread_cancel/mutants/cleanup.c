/*
 * cleanup.c - the cleanup-handler fault of the pthread_cancel faults (CI
 * only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * The pthread_cleanup_push() and pthread_cleanup_pop() macros register
 * each handler through a function of the C library: glibc's
 * __pthread_register_cancel() and __pthread_unregister_cancel(), musl's
 * _pthread_cleanup_push(). These are wrapped here, in a file that does
 * not include <pthread.h> (which declares them with private types).
 */
#define _GNU_SOURCE 1
#include <dlfcn.h>
#include <stddef.h>
#include <string.h>

const char *pthread_cancel_fault(void);

#define FAULT(name) (pthread_cancel_fault() != NULL && strcmp(pthread_cancel_fault(), name) == 0)

static void noop(void *p)
{
	(void)p;
}

/* no_cleanup: a pushed handler never reaches the thread's list. */
void __pthread_register_cancel(void *buf)
{
	static void (*real)(void *);

	if (real == NULL)
		real = (void (*)(void *))dlsym(RTLD_NEXT, "__pthread_register_cancel");
	if (FAULT("no_cleanup") || real == NULL)
		return;
	real(buf);
}

void __pthread_unregister_cancel(void *buf)
{
	static void (*real)(void *);

	if (real == NULL)
		real = (void (*)(void *))dlsym(RTLD_NEXT, "__pthread_unregister_cancel");
	if (FAULT("no_cleanup") || real == NULL)
		return;
	real(buf);
}

void _pthread_cleanup_push(void *cb, void (*f)(void *), void *x)
{
	static void (*real)(void *, void (*)(void *), void *);

	if (real == NULL)
		real = (void (*)(void *, void (*)(void *), void *))dlsym(RTLD_NEXT, "_pthread_cleanup_push");
	if (FAULT("no_cleanup"))
		f = noop;	/* the handler's routine is lost */
	if (real != NULL)
		real(cb, f, x);
}
