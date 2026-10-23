/*
 * raise.c - planted faults in raise(), for mutation checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "raise.<fault>" and apply only while
 * PC_PHASE is "test".
 */
#define _GNU_SOURCE 1
#include <dlfcn.h>
#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

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

int raise(int sig)
{
	static int (*real)(int);
	const char *m = mutant_of("raise.");

	if (real == NULL)
		real = (int (*)(int))dlsym(RTLD_NEXT, "raise");
	if (m == NULL)
		return real(sig);
	if (IS("einval_accepted") && (sig <= 0 || sig > 64))
		return 0;
	if (IS("to_process"))
		return kill(getpid(), sig) == 0 ? 0 : -1;
	if (IS("returns_early") && sig > 0 && sig < 65) {
		/* sent, but held back until after raise() returns (never, here) */
		sigset_t one;
		sigemptyset(&one);
		sigaddset(&one, sig);
		(void)pthread_sigmask(SIG_BLOCK, &one, NULL);
		return pthread_kill(pthread_self(), sig) == 0 ? 0 : -1;
	}
	return real(sig);
}
