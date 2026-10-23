/*
 * sigsuspend.c - planted faults in sigsuspend(), for mutation checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "sigsuspend.<fault>" and apply only while
 * PC_PHASE is "test" (as set by pc.c in this process or a parent).
 */
#define _GNU_SOURCE 1
#include <dlfcn.h>
#include <pthread.h>
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/wait.h>
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
/* The next definition of fn (type is documentation only). */
#define REAL(fn, type) static __typeof__(fn) *real_##fn; \
	if (real_##fn == NULL) real_##fn = (__typeof__(fn) *)dlsym(RTLD_NEXT, #fn)

/* The highest-numbered signal in set that is pending, or 0. */
static __attribute__((unused)) int highest_pending(const sigset_t *set)
{
	sigset_t p;
	int s;

	if (sigpending(&p) != 0)
		return 0;
	for (s = 64; s > 0; s--)
		if (sigismember(set, s) == 1 && sigismember(&p, s) == 1)
			return s;
	return 0;
}

/* The highest-numbered pending signal of any kind, or 0. */
static __attribute__((unused)) int highest_any(void)
{
	sigset_t all;

	sigfillset(&all);
	return highest_pending(&all);
}

/* Takes one pending occurrence of sig without waiting; 0 if none. */
static __attribute__((unused)) int take(int sig, siginfo_t *info)
{
	REAL(sigtimedwait, int (*)(const sigset_t *, siginfo_t *, const struct timespec *));
	sigset_t one;
	struct timespec zero = { 0, 0 };

	sigemptyset(&one);
	sigaddset(&one, sig);
	return real_sigtimedwait(&one, info, &zero) == sig;
}

int sigsuspend(const sigset_t *mask)
{
	REAL(sigsuspend, int (*)(const sigset_t *));
	const char *m = mutant_of("sigsuspend.");
	sigset_t none;
	int r;

	if (m == NULL)
		return real_sigsuspend(mask);
	if (IS("no_wait")) {
		errno = EINTR;
		return -1;
	}
	if (IS("mask_ignored")) {
		sigemptyset(&none);
		mask = &none;
	}
	if (IS("mask_union")) {
		/* adds the new mask to the old one instead of replacing it */
		int s;
		(void)sigprocmask(SIG_BLOCK, NULL, &none);
		for (s = 1; s < 65; s++)
			if (sigismember(mask, s) == 1)
				sigaddset(&none, s);
		mask = &none;
	}
	if (IS("reject_unblockable") && (sigismember(mask, SIGKILL) == 1 || sigismember(mask, SIGSTOP) == 1)) {
		errno = EINVAL;
		return -1;
	}
	if (IS("steals_pending")) {
		/* moves process-pending signals the mask blocks onto this thread */
		int s;
		for (s = 1; s < 65; s++)
			if (sigismember(mask, s) == 1 && take(s, NULL))
				(void)pthread_kill(pthread_self(), s);
	}
	r = real_sigsuspend(mask);
	if (IS("mask_not_restored"))
		(void)sigprocmask(SIG_SETMASK, mask, NULL);
	if (IS("returns_zero"))
		return 0;
	if (IS("errno_not_eintr"))
		errno = EAGAIN;
	return r;
}
