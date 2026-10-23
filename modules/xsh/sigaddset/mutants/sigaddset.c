/*
 * sigaddset.c - planted faults in sigaddset(), for mutation checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "sigaddset.<fault>" and apply only while
 * PC_PHASE is "test" (as set by pc.c in this process or a parent).
 */
#define _GNU_SOURCE 1
#include <dlfcn.h>
#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wchar.h>

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
#define REAL(fn) static __typeof__(fn) *real_##fn; \
	if (real_##fn == NULL) real_##fn = (__typeof__(fn) *)dlsym(RTLD_NEXT, #fn)

int sigaddset(sigset_t *set, int signo)
{
	REAL(sigaddset);
	const char *m = mutant_of("sigaddset.");
	int r;

	if (m == NULL)
		return real_sigaddset(set, signo);
	if (IS("no_effect"))
		return 0;
	if (IS("wrong_bit"))
		return real_sigaddset(set, signo == SIGUSR1 ? SIGUSR2 : signo);
	if (IS("rt_wrong_bit") && signo > SIGRTMIN && signo <= SIGRTMAX)
		return real_sigaddset(set, signo - 1);
	if (IS("clears_others"))
		(void)sigemptyset(set);
	r = real_sigaddset(set, signo);
	if (IS("returns_one") && r == 0)
		return 1;
	return r;
}
