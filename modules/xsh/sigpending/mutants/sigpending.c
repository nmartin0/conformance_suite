/*
 * sigpending.c - planted faults in sigpending(), for mutation checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "sigpending.<fault>" and apply only while
 * PC_PHASE is "test" (as set by pc.c in this process or a parent).
 */
#define _GNU_SOURCE 1
#include <dlfcn.h>
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
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

int sigpending(sigset_t *set)
{
	REAL(sigpending, int (*)(sigset_t *));
	static sigset_t seen;
	static int init;
	const char *m = mutant_of("sigpending.");
	int r = real_sigpending(set), s;

	if (!init) {
		sigemptyset(&seen);
		init = 1;
	}
	if (r != 0 || m == NULL)
		return r;
	if (IS("empty"))
		sigemptyset(set);
	if (IS("full"))
		sigaddset(set, SIGTERM);
	if (IS("sticky_union"))
		for (s = 1; s < 65; s++) {	/* never forgets a signal it has reported */
			if (sigismember(set, s) == 1)
				sigaddset(&seen, s);
			if (sigismember(&seen, s) == 1)
				sigaddset(set, s);
		}
	return r;
}
