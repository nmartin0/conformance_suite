/*
 * alarm.c - planted faults in alarm(), for mutation checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "alarm.<fault>" and apply only while
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

static unsigned left(void)
{
	struct itimerval it;

	if (getitimer(ITIMER_REAL, &it) != 0)
		return 0;
	if (it.it_value.tv_sec == 0 && it.it_value.tv_usec == 0)
		return 0;
	return (unsigned)it.it_value.tv_sec + (it.it_value.tv_usec > 0);
}

unsigned alarm(unsigned sec)
{
	REAL(alarm, unsigned (*)(unsigned));
	const char *m = mutant_of("alarm.");
	unsigned r;

	if (m == NULL)
		return real_alarm(sec);
	if (IS("cancel_ignored") && sec == 0)
		return left();
	if (IS("no_reschedule") && left() != 0)
		return left();
	if (IS("never_fires"))
		return real_alarm(0);
	if (IS("fires_early") && sec > 0) {
		struct itimerval it;
		r = real_alarm(0);
		memset(&it, 0, sizeof it);
		it.it_value.tv_sec = sec - 1;
		it.it_value.tv_usec = 700000;
		(void)setitimer(ITIMER_REAL, &it, NULL);
		return r;
	}
	if (IS("stacked") && sec > 0 && left() != 0) {
		/* the earlier alarm still goes off too, from a helper */
		pid_t me = getpid(), c = fork();
		if (c == 0) {
			struct timespec ts = { (time_t)sec, 300000000L };
			while (nanosleep(&ts, &ts) == -1 && errno == EINTR)
				;
			(void)kill(me, SIGALRM);
			_exit(0);
		}
	}
	r = real_alarm(sec);
	if (IS("returns_zero"))
		return 0;
	if (IS("returns_nonzero"))
		return r + 5;
	return r;
}
