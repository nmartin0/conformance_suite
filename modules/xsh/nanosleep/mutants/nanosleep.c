/*
 * nanosleep.c - planted faults in nanosleep(), for mutation checks (CI
 * only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "nanosleep.<fault>" and apply only
 * while PC_PHASE is "test" (as set by pc.c in this process or a parent).
 */
#define _GNU_SOURCE 1
#include <dlfcn.h>
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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

static long long ns(const struct timespec *t)
{
	return (long long)t->tv_sec * 1000000000LL + t->tv_nsec;
}

static struct timespec ts(long long n)
{
	struct timespec t;

	if (n < 0)
		n = 0;
	t.tv_sec = (time_t)(n / 1000000000LL);
	t.tv_nsec = (long)(n % 1000000000LL);
	return t;
}

static long long now(void)
{
	struct timespec t;

	(void)clock_gettime(CLOCK_MONOTONIC, &t);
	return ns(&t);
}

/*
 * "process_wide": every caught signal, whichever thread it is delivered
 * to, ends the sleep. Handlers are wrapped (whatever the phase, since
 * tests install them during setup) so that their calls can be counted.
 */
static volatile sig_atomic_t handled;
static void (*user_handler[65])(int);
static void (*user_action[65])(int, siginfo_t *, void *);

static void tramp(int sig)
{
	handled++;
	if (sig > 0 && sig < 65 && user_handler[sig] != NULL)
		user_handler[sig](sig);
}

static void tramp_info(int sig, siginfo_t *si, void *uc)
{
	handled++;
	if (sig > 0 && sig < 65 && user_action[sig] != NULL)
		user_action[sig](sig, si, uc);
}

int sigaction(int sig, const struct sigaction *act, struct sigaction *old)
{
	REAL(sigaction);
	const char *m = getenv("PC_MUTANT");
	struct sigaction a;
	int r;

	if (m == NULL || strcmp(m, "nanosleep.process_wide") != 0 || sig <= 0 || sig >= 65)
		return real_sigaction(sig, act, old);
	if (act != NULL) {
		a = *act;
		if (act->sa_flags & SA_SIGINFO) {
			user_action[sig] = act->sa_sigaction;
			a.sa_sigaction = tramp_info;
		} else if (act->sa_handler != SIG_DFL && act->sa_handler != SIG_IGN) {
			user_handler[sig] = act->sa_handler;
			a.sa_handler = tramp;
		}
		act = &a;
	}
	r = real_sigaction(sig, act, old);
	if (r == 0 && old != NULL) {
		if ((old->sa_flags & SA_SIGINFO) && old->sa_sigaction == tramp_info)
			old->sa_sigaction = user_action[sig];
		else if (old->sa_handler == tramp)
			old->sa_handler = user_handler[sig];
	}
	return r;
}

static void noop(int sig)
{
	(void)sig;
}

/* Sleeps in 10 ms slices while stop() says no; the time left, or -1 when stopped. */
static int sliced(const struct timespec *rq, struct timespec *rm, int (*stop)(void))
{
	REAL(nanosleep);
	long long end = now() + ns(rq), left;
	struct timespec slice;

	while ((left = end - now()) > 0) {
		if (stop()) {
			if (rm != NULL)
				*rm = ts(left);
			errno = EINTR;
			return -1;
		}
		slice = ts(left < 10000000LL ? left : 10000000LL);
		(void)real_nanosleep(&slice, NULL);
	}
	return 0;
}

static int any_pending(void)
{
	sigset_t p;
	int s;

	if (sigpending(&p) != 0)
		return 0;
	for (s = 1; s < 65; s++)
		if (sigismember(&p, s) == 1)
			return 1;
	return 0;
}

static sig_atomic_t handled0;

static int any_handled(void)
{
	return handled != handled0;
}

int nanosleep(const struct timespec *rq, struct timespec *rm)
{
	REAL(nanosleep);
	REAL(sigaction);
	const char *m = mutant_of("nanosleep.");
	struct timespec t, mine, req;
	int r, e;

	if (m == NULL)
		return real_nanosleep(rq, rm);
	req = *rq;
	if (IS("short") && rq->tv_nsec >= 0 && rq->tv_nsec < 1000000000L) {
		t = ts(ns(rq) / 2);
		return real_nanosleep(&t, rm);
	}
	if (IS("ignores_sec") && rq->tv_nsec >= 0 && rq->tv_nsec < 1000000000L) {
		t.tv_sec = 0;
		t.tv_nsec = rq->tv_nsec;
		return real_nanosleep(&t, rm);
	}
	if (IS("nsec_carried") && (rq->tv_nsec < 0 || rq->tv_nsec >= 1000000000L)) {
		t = ts(ns(rq));
		return real_nanosleep(&t, rm);
	}
	if (IS("nsec_neg_ok") && rq->tv_nsec < 0) {
		t.tv_sec = rq->tv_sec;
		t.tv_nsec = 0;
		return real_nanosleep(&t, rm);
	}
	if (IS("same_clobbered") && rm == rq) {
		/* the remaining time is cleared before the request is read */
		memset(rm, 0, sizeof *rm);
		return real_nanosleep(rq, rm);
	}
	if (IS("blocked_wakes"))
		return sliced(rq, rm, any_pending);
	if (IS("process_wide")) {
		handled0 = handled;
		return sliced(rq, rm, any_handled);
	}
	if (IS("ignored_wakes")) {
		/* an ignored signal is caught for the length of the sleep */
		struct sigaction was[32], sa;
		int s;

		memset(&sa, 0, sizeof sa);
		sa.sa_handler = noop;
		sigemptyset(&sa.sa_mask);
		for (s = 1; s < 32; s++) {
			if (s == SIGKILL || s == SIGSTOP || real_sigaction(s, NULL, &was[s]) != 0)
				continue;
			if (!(was[s].sa_flags & SA_SIGINFO) && was[s].sa_handler == SIG_IGN)
				(void)real_sigaction(s, &sa, NULL);
		}
		r = real_nanosleep(rq, rm);
		e = errno;
		for (s = 1; s < 32; s++)
			if (s != SIGKILL && s != SIGSTOP && !(was[s].sa_flags & SA_SIGINFO) && was[s].sa_handler == SIG_IGN)
				(void)real_sigaction(s, &was[s], NULL);
		errno = e;
		return r;
	}
	if (IS("alarm_action_left") || IS("alarm_blocked_left")) {
		/* built on SIGALRM, and the old state is not put back */
		struct sigaction sa;
		sigset_t set;

		memset(&sa, 0, sizeof sa);
		sa.sa_handler = noop;
		sigemptyset(&sa.sa_mask);
		if (IS("alarm_action_left"))
			(void)real_sigaction(SIGALRM, &sa, NULL);
		sigemptyset(&set);
		sigaddset(&set, SIGALRM);
		if (IS("alarm_blocked_left"))
			(void)sigprocmask(SIG_BLOCK, &set, NULL);
		return real_nanosleep(rq, rm);
	}
	if ((IS("null_restarts") || IS("restarts")) && (rm == NULL || IS("restarts"))) {
		t = *rq;
		while ((r = real_nanosleep(&t, &t)) == -1 && errno == EINTR)
			;
		return r;
	}
	r = real_nanosleep(rq, IS("rem_untouched") ? &mine : rm);
	e = errno;
	if (IS("spurious_eintr") && r == 0) {
		if (rm != NULL)
			rm->tv_sec = rm->tv_nsec = 0;
		errno = EINTR;
		return -1;
	}
	if (r == -1 && e == EINTR) {
		if (IS("rem_zero") && rm != NULL)
			rm->tv_sec = rm->tv_nsec = 0;
		if (IS("rem_elapsed") && rm != NULL)
			*rm = ts(ns(&req) - ns(rm));	/* the time slept instead */
		if (IS("eintr_zero"))
			return 0;
		if (IS("null_efault") && rm == NULL) {
			errno = EFAULT;
			return -1;
		}
	}
	errno = e;
	return r;
}
