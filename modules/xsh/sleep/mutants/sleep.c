/*
 * sleep.c - planted faults in sleep(), for mutation checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "sleep.<fault>" and apply only while
 * PC_PHASE is "test" (as set by pc.c in this process or a parent).
 */
#define _GNU_SOURCE 1
#include <dlfcn.h>
#include <errno.h>
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
#define REAL(fn) static __typeof__(fn) *real_##fn; \
	if (real_##fn == NULL) real_##fn = (__typeof__(fn) *)dlsym(RTLD_NEXT, #fn)

static long long mono(void)
{
	struct timespec t;

	(void)clock_gettime(CLOCK_MONOTONIC, &t);
	return (long long)t.tv_sec * 1000000000LL + t.tv_nsec;
}

static void nap_ms(long ms)
{
	struct timespec t;

	t.tv_sec = ms / 1000;
	t.tv_nsec = (ms % 1000) * 1000000L;
	while (clock_nanosleep(CLOCK_MONOTONIC, 0, &t, &t) == EINTR)
		;
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

static volatile sig_atomic_t rang;

static void ring(int sig)
{
	(void)sig;
	rang = 1;
}

static void noop(int sig)
{
	(void)sig;
}

unsigned sleep(unsigned sec)
{
	REAL(sleep);
	const char *m = mutant_of("sleep.");
	unsigned r;

	if (m == NULL)
		return real_sleep(sec);
	if (IS("noop"))
		return 0;
	if (IS("early") && sec > 0) {
		nap_ms((long)sec * 1000L - 300L);
		return 0;
	}
	if (IS("restarts")) {
		long long end = mono() + (long long)sec * 1000000000LL;

		while ((r = real_sleep(sec)) != 0 && mono() < end)
			sec = r;
		return 0;
	}
	if (IS("blocked_wakes")) {
		long long end = mono() + (long long)sec * 1000000000LL, left;

		while ((left = end - mono()) > 0) {
			if (any_pending())
				return (unsigned)((left + 999999999LL) / 1000000000LL);
			nap_ms(10);
		}
		return 0;
	}
	if (IS("ignored_wakes")) {
		/* an ignored signal is caught for the length of the sleep */
		struct sigaction was[32], sa;
		int s;

		memset(&sa, 0, sizeof sa);
		sa.sa_handler = noop;
		sigemptyset(&sa.sa_mask);
		for (s = 1; s < 32; s++) {
			if (s == SIGKILL || s == SIGSTOP || sigaction(s, NULL, &was[s]) != 0)
				continue;
			if (!(was[s].sa_flags & SA_SIGINFO) && was[s].sa_handler == SIG_IGN)
				(void)sigaction(s, &sa, NULL);
		}
		r = real_sleep(sec);
		for (s = 1; s < 32; s++)
			if (s != SIGKILL && s != SIGSTOP && !(was[s].sa_flags & SA_SIGINFO) && was[s].sa_handler == SIG_IGN)
				(void)sigaction(s, &was[s], NULL);
		return r;
	}
	if (IS("alarm_replaced")) {
		/* built on alarm(), and an earlier alarm is lost */
		(void)alarm(0);
		return real_sleep(sec);
	}
	if (IS("alarm_to_handler") && sec > 0) {
		/* built on alarm(), without a handler of its own */
		unsigned prev = alarm(0);
		(void)alarm(sec);
		r = real_sleep(sec + 1);
		(void)alarm(prev > sec ? prev - sec : (prev ? 1 : 0));
		return 0;
	}
	if (IS("alarm_action_left") && sec > 0) {
		/* built on alarm() with its own handler, which it leaves in place */
		struct sigaction sa;
		unsigned prev = alarm(0);
		long long cap = mono() + ((long long)sec + 2) * 1000000000LL;

		memset(&sa, 0, sizeof sa);
		sa.sa_handler = ring;
		sigemptyset(&sa.sa_mask);
		(void)sigaction(SIGALRM, &sa, NULL);
		rang = 0;
		(void)alarm(sec);
		while (!rang && mono() < cap)
			nap_ms(10);
		(void)alarm(prev > sec ? prev - sec : (prev ? 1 : 0));
		return 0;
	}
	r = real_sleep(sec);
	if (IS("returns_nonzero") && r == 0)
		return 1;
	if (IS("returns_slept") && r != 0)
		return sec - r;
	if (IS("returns_zero"))
		return 0;
	return r;
}
