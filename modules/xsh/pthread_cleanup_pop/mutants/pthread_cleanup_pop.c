/*
 * pthread_cleanup_pop.c - planted faults in pthread_cleanup_push() and
 * pthread_cleanup_pop(), for mutation checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "pthread_cleanup_pop.<fault>" and
 * apply only while PC_PHASE is "test" (as set by pc.c in this process).
 *
 * The two are macros on the reference systems (the page allows macros or
 * functions), so a fault can reach only the C library functions the
 * macros call: musl's _pthread_cleanup_push() and _pthread_cleanup_pop(),
 * glibc's __pthread_register_cancel() and __pthread_unregister_cancel().
 * glibc's pthread_cleanup_pop() calls the handler inline, so the faults
 * in what pop(execute) runs cannot be planted there: those runs say so.
 * This file does not include <pthread.h>, which declares the functions
 * with private types.
 */
#define _GNU_SOURCE 1
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char phase[16];

/* pc.c sets PC_PHASE with setenv(); remember it in this process. */
int setenv(const char *name, const char *value, int overwrite)
{
	static int (*real)(const char *, const char *, int);

	if (real == NULL)
		real = (int (*)(const char *, const char *, int))dlsym(RTLD_NEXT, "setenv");
	if (strcmp(name, "PC_PHASE") == 0 && strlen(value) < sizeof phase)
		strcpy(phase, value);
	return real(name, value, overwrite);
}

static const char *mutant_of(const char *prefix)
{
	const char *m = getenv("PC_MUTANT");
	size_t n = strlen(prefix);

	if (strcmp(phase, "test") != 0 || m == NULL || strncmp(m, prefix, n) != 0)
		return NULL;
	return m + n;
}

#define IS(name) (m != NULL && strcmp(m, name) == 0)

static void noop(void *p)
{
	(void)p;
}

/* musl */
void _pthread_cleanup_push(void *cb, void (*f)(void *), void *x)
{
	const char *m = mutant_of("pthread_cleanup_pop.");
	static void (*real)(void *, void (*)(void *), void *);

	if (real == NULL)
		real = (void (*)(void *, void (*)(void *), void *))dlsym(RTLD_NEXT, "_pthread_cleanup_push");
	if (IS("no_cleanup"))
		f = noop;	/* the routine is lost: nothing runs, at pop, exit or cancel */
	real(cb, f, x);
}

void _pthread_cleanup_pop(void *cb, int run)
{
	const char *m = mutant_of("pthread_cleanup_pop.");
	static void (*real)(void *, int);

	if (real == NULL)
		real = (void (*)(void *, int))dlsym(RTLD_NEXT, "_pthread_cleanup_pop");
	if (IS("runs_on_zero"))
		run = 1;	/* execute is ignored: the handler always runs */
	if (IS("never_runs"))
		run = 0;
	real(cb, run);
}

/* glibc */
void __pthread_register_cancel(void *buf)
{
	const char *m = mutant_of("pthread_cleanup_pop.");
	static void (*real)(void *);
	static int said;

	if (real == NULL)
		real = (void (*)(void *))dlsym(RTLD_NEXT, "__pthread_register_cancel");
	if ((IS("runs_on_zero") || IS("never_runs")) && !said) {
		said = 1;
		(void)fprintf(stderr, "pcmutant: cannot plant pthread_cleanup_pop.%s here: the macro calls the handler inline\n", m);
	}
	if (IS("no_cleanup"))
		return;		/* never on the thread's list */
	real(buf);
}

void __pthread_unregister_cancel(void *buf)
{
	const char *m = mutant_of("pthread_cleanup_pop.");
	static void (*real)(void *);

	if (real == NULL)
		real = (void (*)(void *))dlsym(RTLD_NEXT, "__pthread_unregister_cancel");
	if (IS("no_cleanup"))
		return;
	real(buf);
}
