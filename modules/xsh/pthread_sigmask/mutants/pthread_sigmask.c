/*
 * pthread_sigmask.c - planted faults in pthread_sigmask() and
 * sigprocmask(), for mutation checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "pthread_sigmask.<fault>" and apply
 * to both functions while PC_PHASE is "test".
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

typedef int (*mask_fn)(int, const sigset_t *, sigset_t *);

/* 0 or an error number, as pthread_sigmask() reports it. */
static int faulty(const char *m, mask_fn real, int is_procmask, int how, const sigset_t *set, sigset_t *oset)
{
	sigset_t s, cur;
	int r;

	if (set == NULL) {
		r = real(how, NULL, oset);
		if (IS("query_changes")) {
			sigemptyset(&s);
			(void)real(SIG_SETMASK, &s, NULL);
		}
		r = is_procmask ? (r == 0 ? 0 : errno) : r;
		goto done;
	}
	if (how != SIG_BLOCK && how != SIG_UNBLOCK && how != SIG_SETMASK && IS("einval_accepted"))
		return 0;
	s = *set;
	if (IS("block_replaces") && how == SIG_BLOCK)
		how = SIG_SETMASK;
	if (IS("block_ignored") && how == SIG_BLOCK)
		return real(SIG_BLOCK, NULL, oset) == 0 ? 0 : EINVAL;
	if (IS("unblock_ignored") && how == SIG_UNBLOCK)
		return real(SIG_BLOCK, NULL, oset) == 0 ? 0 : EINVAL;
	if (IS("unblock_all") && how == SIG_UNBLOCK) {
		how = SIG_SETMASK;
		sigemptyset(&s);
	}
	if (IS("setmask_union") && how == SIG_SETMASK)
		how = SIG_BLOCK;
	if (IS("unblock_deferred") && how == SIG_UNBLOCK) {
		/* pending signals being unblocked are dropped, not delivered */
		sigset_t p, one;
		struct timespec zero = { 0, 0 };
		int sig;
		if (sigpending(&p) == 0)
			for (sig = 1; sig < 65; sig++)
				if (sigismember(&p, sig) == 1 && sigismember(&s, sig) == 1) {
					sigemptyset(&one);
					sigaddset(&one, sig);
					(void)sigtimedwait(&one, NULL, &zero);
				}
	}
	(void)real(SIG_BLOCK, NULL, &cur);
	r = real(how, &s, oset);
	if (is_procmask)
		r = r == 0 ? 0 : errno;
done:
	if (r == 0 && oset != NULL && IS("oset_lost"))
		sigemptyset(oset);
	(void)cur;
	return r;
}

int pthread_sigmask(int how, const sigset_t *set, sigset_t *oset)
{
	static mask_fn real;
	const char *m = mutant_of("pthread_sigmask.");
	int r;

	if (real == NULL)
		real = (mask_fn)dlsym(RTLD_NEXT, "pthread_sigmask");
	if (m == NULL)
		return real(how, set, oset);
	r = faulty(m, real, 0, how, set, oset);
	if (r != 0 && IS("returns_minus1")) {
		errno = r;
		return -1;
	}
	return r;
}

int sigprocmask(int how, const sigset_t *set, sigset_t *oset)
{
	static mask_fn real;
	const char *m = mutant_of("pthread_sigmask.");
	int r;

	if (real == NULL)
		real = (mask_fn)dlsym(RTLD_NEXT, "sigprocmask");
	if (m == NULL)
		return real(how, set, oset);
	r = faulty(m, real, 1, how, set, oset);
	if (r != 0) {
		errno = r;
		return -1;
	}
	return 0;
}
