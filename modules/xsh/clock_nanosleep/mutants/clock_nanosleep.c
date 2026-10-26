/*
 * clock_nanosleep.c - planted faults in clock_nanosleep(), for mutation
 * checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "clock_nanosleep.<fault>" and apply
 * only while PC_PHASE is "test" (as set by pc.c in this process or a
 * parent).
 */
#define _GNU_SOURCE 1
#include <dlfcn.h>
#include <errno.h>
#include <pthread.h>
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

static long long now(clockid_t id)
{
	struct timespec t;

	if (clock_gettime(id, &t) != 0)
		return 0;
	return ns(&t);
}

static void noop(int sig)
{
	(void)sig;
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

static int valid(const struct timespec *t)
{
	return t->tv_nsec >= 0 && t->tv_nsec < 1000000000L;
}

int clock_nanosleep(clockid_t id, int flags, const struct timespec *rq, struct timespec *rm)
{
	REAL(clock_nanosleep);
	REAL(sigaction);
	const char *m = mutant_of("clock_nanosleep.");
	int abs = (flags & TIMER_ABSTIME) != 0, r;
	struct timespec t, mine, req;
	struct timespec probe;

	if (m == NULL)
		return real_clock_nanosleep(id, flags, rq, rm);
	req = *rq;
	if (IS("short") && !abs && valid(rq)) {
		t = ts(ns(rq) / 2);
		return real_clock_nanosleep(id, flags, &t, rm);
	}
	if (IS("ignores_sec") && !abs && valid(rq)) {
		t.tv_sec = 0;
		t.tv_nsec = rq->tv_nsec;
		return real_clock_nanosleep(id, flags, &t, rm);
	}
	if (IS("abs_early") && abs && valid(rq)) {
		t = ts(ns(rq) - 100000000LL);
		return real_clock_nanosleep(id, flags, &t, rm);
	}
	if (IS("abs_ignored") && abs && valid(rq))
		return 0;
	if (IS("past_sleeps") && abs && valid(rq) && ns(rq) <= now(id)) {
		/* the distance to a past time is slept as if it were ahead */
		t = ts(now(id) - ns(rq) + 1000000000LL);
		return real_clock_nanosleep(id, 0, &t, NULL);
	}
	if (IS("past_einval") && abs && valid(rq) && ns(rq) <= now(id))
		return EINVAL;
	if (IS("nsec_carried") && !valid(rq)) {
		t = ts(ns(rq));
		return real_clock_nanosleep(id, flags, &t, rm);
	}
	if (IS("nsec_neg_ok") && rq->tv_nsec < 0) {
		t.tv_sec = rq->tv_sec;
		t.tv_nsec = 0;
		return real_clock_nanosleep(id, flags, &t, rm);
	}
	if ((IS("unknown_enotsup") || IS("unknown_ok")) && clock_gettime(id, &probe) != 0) {
		if (IS("unknown_enotsup"))
			return ENOTSUP;
		return real_clock_nanosleep(CLOCK_MONOTONIC, 0, &req, rm);
	}
	if (IS("thread_cpu_ok") || IS("thread_cpu_eperm")) {
		clockid_t self;

		if (id == CLOCK_THREAD_CPUTIME_ID || (pthread_getcpuclockid(pthread_self(), &self) == 0 && id == self)) {
			if (IS("thread_cpu_eperm"))
				return EPERM;
			t.tv_sec = 0;
			t.tv_nsec = 10000000L;
			return real_clock_nanosleep(CLOCK_MONOTONIC, 0, &t, NULL);
		}
	}
	if (IS("same_clobbered") && rm == rq) {
		memset(rm, 0, sizeof *rm);
		return real_clock_nanosleep(id, flags, rq, rm);
	}
	if (IS("blocked_wakes") && !abs && valid(rq)) {
		long long end = now(CLOCK_MONOTONIC) + ns(rq), left;

		while ((left = end - now(CLOCK_MONOTONIC)) > 0) {
			if (any_pending()) {
				if (rm != NULL)
					*rm = ts(left);
				return EINTR;
			}
			t = ts(left < 10000000LL ? left : 10000000LL);
			(void)real_clock_nanosleep(CLOCK_MONOTONIC, 0, &t, NULL);
		}
		return 0;
	}
	if (IS("ignored_wakes")) {
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
		r = real_clock_nanosleep(id, flags, rq, rm);
		for (s = 1; s < 32; s++)
			if (s != SIGKILL && s != SIGSTOP && !(was[s].sa_flags & SA_SIGINFO) && was[s].sa_handler == SIG_IGN)
				(void)real_sigaction(s, &was[s], NULL);
		return r;
	}
	if (IS("alarm_action_left") || IS("alarm_blocked_left")) {
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
		return real_clock_nanosleep(id, flags, rq, rm);
	}
	if (IS("restarts") || (IS("null_restarts") && rm == NULL && !abs)) {
		t = req;
		while ((r = real_clock_nanosleep(id, flags, &t, abs ? NULL : &t)) == EINTR)
			if (abs)
				t = req;
		return r;
	}
	if (IS("abs_rem_written") && abs) {
		r = real_clock_nanosleep(id, flags, rq, NULL);
		if (r == EINTR && rm != NULL)
			*rm = ts(ns(&req) - now(id));
		return r;
	}
	r = real_clock_nanosleep(id, flags, rq, IS("rem_untouched") ? &mine : rm);
	if (IS("spurious_eintr") && r == 0 && !abs) {
		if (rm != NULL)
			rm->tv_sec = rm->tv_nsec = 0;
		return EINTR;
	}
	if (r == EINTR && !abs) {
		if (IS("rem_zero") && rm != NULL)
			rm->tv_sec = rm->tv_nsec = 0;
		if (IS("rem_elapsed") && rm != NULL)
			*rm = ts(ns(&req) - ns(rm));
		if (IS("null_efault") && rm == NULL)
			return EFAULT;
	}
	if (IS("returns_minus1") && r != 0) {
		/* errors reported the nanosleep() way */
		errno = r;
		return -1;
	}
	return r;
}
