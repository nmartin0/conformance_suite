/*
 * exit.c - planted faults in exit(), for mutation checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "exit.<fault>" and apply only while
 * PC_PHASE is "test". While a fault is planted, functions registered with
 * atexit() (glibc routes it through __cxa_atexit()) or at_quick_exit()
 * are recorded here instead of in the C library, and exit() runs them
 * itself, so the order, the flushing and the rest can be got wrong on
 * purpose.
 */
#define _GNU_SOURCE 1
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef void (*exit_fn)(int);
typedef int (*cxa_fn)(void (*)(void *), void *, void *);
typedef int (*atexit_fn)(void (*)(void));

static const char *mutant(void)
{
	const char *ph = getenv("PC_PHASE");
	const char *m = getenv("PC_MUTANT");
	if (ph == NULL || strcmp(ph, "test") != 0 || m == NULL ||
	    strncmp(m, "exit.", 5) != 0)
		return NULL;
	return m + 5;
}

#define IS(name) (m != NULL && strcmp(m, name) == 0)

static struct { void (*f)(void *); void *arg; int plain; } list[64], quick[64];
static int n, nq;

static void call_plain(void *f)
{
	((void (*)(void))f)();
}

int __cxa_atexit(void (*f)(void *), void *arg, void *dso)
{
	static cxa_fn real;
	const char *m = mutant();

	if (real == NULL)
		real = (cxa_fn)dlsym(RTLD_NEXT, "__cxa_atexit");
	if (m == NULL || n >= 64)
		return real ? real(f, arg, dso) : -1;
	list[n].f = f;
	list[n].arg = arg;
	n++;
	return 0;
}

int atexit(void (*f)(void))
{
	static atexit_fn real;
	const char *m = mutant();

	if (real == NULL)
		real = (atexit_fn)dlsym(RTLD_NEXT, "atexit");
	if (m == NULL || n >= 64)
		return real ? real(f) : __cxa_atexit(call_plain, (void *)f, NULL);
	list[n].f = call_plain;
	list[n].arg = (void *)f;
	n++;
	return 0;
}

/* glibc's at_quick_exit() lives in the program and calls this one. */
int __cxa_at_quick_exit(void (*f)(void *), void *dso)
{
	static int (*real)(void (*)(void *), void *);
	const char *m = mutant();

	if (real == NULL)
		real = (int (*)(void (*)(void *), void *))dlsym(RTLD_NEXT, "__cxa_at_quick_exit");
	if (m == NULL || nq >= 64)
		return real ? real(f, dso) : -1;
	quick[nq].f = f;
	quick[nq].arg = NULL;
	nq++;
	return 0;
}

int at_quick_exit(void (*f)(void))
{
	static atexit_fn real;
	const char *m = mutant();

	if (real == NULL)
		real = (atexit_fn)dlsym(RTLD_NEXT, "at_quick_exit");
	if (m == NULL || nq >= 64)
		return real ? real(f) : -1;
	quick[nq].f = call_plain;
	quick[nq].arg = (void *)f;
	nq++;
	return 0;
}

void exit(int status)
{
	static exit_fn real;
	const char *m = mutant();
	int i;

	if (real == NULL)
		real = (exit_fn)dlsym(RTLD_NEXT, "exit");
	if (m == NULL)
		real(status);
	if (IS("status_wrong"))
		status ^= 1;
	if (IS("no_atexit"))
		_exit(status);
	if (IS("flush_first"))
		(void)fflush(NULL);
	if (IS("calls_quick"))
		for (i = nq - 1; i >= 0; i--)
			quick[i].f(quick[i].arg);
	if (IS("forward_order")) {
		for (i = 0; i < n; i++)
			list[i].f(list[i].arg);
	} else if (IS("late_registration_skipped")) {
		int top = n;
		for (i = top - 1; i >= 0; i--)
			list[i].f(list[i].arg);	/* ignores what is added meanwhile */
	} else if (IS("once_each")) {
		for (i = n - 1; i >= 0; i--) {
			int j, seen = 0;
			for (j = i + 1; j < n; j++)
				if (list[j].f == list[i].f && list[j].arg == list[i].arg)
					seen = 1;
			if (!seen)
				list[i].f(list[i].arg);
		}
	} else {
		while (n > 0) {
			n--;
			list[n].f(list[n].arg);	/* handlers may register more */
		}
	}
	if (IS("no_flush"))
		_exit(status);
	real(status);
	_exit(status);	/* not reached */
}
