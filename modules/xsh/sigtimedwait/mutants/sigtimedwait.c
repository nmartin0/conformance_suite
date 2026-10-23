/*
 * sigtimedwait.c - planted faults in sigtimedwait() and sigwaitinfo(), for mutation checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "sigtimedwait.<fault>" and apply only while
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

static int after(const char *m, int r, siginfo_t *info)
{
	if (r <= 0)
		return r;
	if (IS("info_lost") && info != NULL)
		info->si_code = 12345;	/* not a code the signal could have */
	if (IS("value_lost") && info != NULL)
		info->si_value.sival_int = 0;
	if (IS("not_cleared"))
		(void)kill(getpid(), r);
	return r;
}

int sigwaitinfo(const sigset_t *set, siginfo_t *info)
{
	REAL(sigwaitinfo, int (*)(const sigset_t *, siginfo_t *));
	const char *m = mutant_of("sigtimedwait.");
	int h;

	if (m == NULL)
		return real_sigwaitinfo(set, info);
	if (IS("highest_first") && (h = highest_pending(set)) != 0 && take(h, info))
		return h;
	return after(m, real_sigwaitinfo(set, info), info);
}

int sigtimedwait(const sigset_t *set, siginfo_t *info, const struct timespec *to)
{
	REAL(sigtimedwait, int (*)(const sigset_t *, siginfo_t *, const struct timespec *));
	const char *m = mutant_of("sigtimedwait.");
	struct timespec t, zero = { 0, 0 };
	int r, h;

	if (m == NULL || to == NULL)
		return after(m, real_sigtimedwait(set, info, to), info);
	t = *to;
	if (IS("einval_accepted") && (t.tv_nsec < 0 || t.tv_nsec >= 1000000000L))
		t = zero;
	if (IS("short_wait"))
		t.tv_nsec /= 2, t.tv_sec /= 2;
	if (IS("zero_fails") && t.tv_sec == 0 && t.tv_nsec == 0) {
		errno = EAGAIN;
		return -1;
	}
	if (IS("timeout_ignored") && t.tv_sec == 0 && t.tv_nsec == 0)
		t.tv_nsec = 200000000L;
	if (IS("highest_first") && (h = highest_pending(set)) != 0 && take(h, info))
		return h;
	if (IS("no_wake")) {
		/* sleeps out the whole timeout before looking */
		struct timespec rem = t;
		while (nanosleep(&rem, &rem) == -1 && errno == EINTR)
			;
		t = zero;
	}
	r = real_sigtimedwait(set, info, &t);
	if (r == -1 && IS("eagain_as_eintr") && errno == EAGAIN)
		errno = EINTR;
	return after(m, r, info);
}
