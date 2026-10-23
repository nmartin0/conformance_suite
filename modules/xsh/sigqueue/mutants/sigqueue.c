/*
 * sigqueue.c - planted faults in sigqueue(), for mutation checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "sigqueue.<fault>" and apply only while
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

int sigqueue(pid_t pid, int sig, const union sigval value)
{
	REAL(sigqueue, int (*)(pid_t, int, const union sigval));
	const char *m = mutant_of("sigqueue.");
	union sigval v = value;
	int r;

	if (m == NULL)
		return real_sigqueue(pid, sig, v);
	if (IS("einval_accepted") && (sig < 0 || sig > 64))
		return 0;
	if (IS("drops") && sig != 0 && pid != getpid())
		return 0;
	if (IS("as_kill") && sig > 0)
		return kill(pid, sig);	/* no value, no queuing */
	if (IS("null_sends") && sig == 0)
		sig = SIGRTMIN;
	if (IS("value_lost"))
		v.sival_int = 0;
	if (IS("deferred") && pid == getpid() && sig > 0) {
		/* held back: blocked, and left blocked */
		sigset_t one;
		sigemptyset(&one);
		sigaddset(&one, sig);
		(void)sigprocmask(SIG_BLOCK, &one, NULL);
	}
	r = real_sigqueue(pid, sig, v);
	if (r == -1 && IS("eagain_never") && errno == EAGAIN)
		return 0;
	if (r == -1 && IS("eperm_hidden") && errno == EPERM)
		return 0;
	if (r == -1 && IS("esrch_as_eperm") && errno == ESRCH)
		errno = EPERM;
	return r;
}
