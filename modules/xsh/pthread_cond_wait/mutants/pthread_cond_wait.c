/*
 * pthread_cond_wait.c - planted faults in pthread_cond_wait(),
 * pthread_cond_timedwait() and pthread_cond_clockwait(), for mutation
 * checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "pthread_cond_wait.<fault>" and
 * apply only while PC_PHASE is "test" (as set by pc.c in this process).
 */
#define _GNU_SOURCE 1
#include <dlfcn.h>
#include <errno.h>
#include <pthread.h>
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

typedef int (*wait_fn)(pthread_cond_t *, pthread_mutex_t *);
typedef int (*timed_fn)(pthread_cond_t *, pthread_mutex_t *, const struct timespec *);
typedef int (*clock_fn)(pthread_cond_t *, pthread_mutex_t *, clockid_t, const struct timespec *);
typedef int (*signal_fn)(pthread_cond_t *);

static wait_fn real_wait;
static timed_fn real_timed;
static clock_fn real_clock;
static signal_fn real_signal;

static void init(void)
{
	if (real_wait == NULL) {
		real_wait = (wait_fn)dlsym(RTLD_NEXT, "pthread_cond_wait");
		real_timed = (timed_fn)dlsym(RTLD_NEXT, "pthread_cond_timedwait");
		real_clock = (clock_fn)dlsym(RTLD_NEXT, "pthread_cond_clockwait");
		real_signal = (signal_fn)dlsym(RTLD_NEXT, "pthread_cond_signal");
	}
}

static void sleep_ms(long ms)
{
	struct timespec ts = { ms / 1000, (ms % 1000) * 1000000L };

	while (nanosleep(&ts, &ts) == -1 && errno == EINTR)
		;
}

static int passed(clockid_t k, const struct timespec *abstime)
{
	struct timespec now;

	(void)clock_gettime(k, &now);
	return now.tv_sec > abstime->tv_sec || (now.tv_sec == abstime->tv_sec && now.tv_nsec >= abstime->tv_nsec);
}

/* abstime on clock "from" as the same instant on clock "to". */
static struct timespec convert(const struct timespec *abstime, clockid_t from, clockid_t to)
{
	struct timespec a, b, r;
	long long ns;

	(void)clock_gettime(from, &a);
	(void)clock_gettime(to, &b);
	ns = (long long)(abstime->tv_sec - a.tv_sec) * 1000000000LL + (abstime->tv_nsec - a.tv_nsec);
	ns += (long long)b.tv_sec * 1000000000LL + b.tv_nsec;
	if (ns < 0)
		ns = 0;
	r.tv_sec = (time_t)(ns / 1000000000LL);
	r.tv_nsec = (long)(ns % 1000000000LL);
	return r;
}

/* cancel_unlocked: a cleanup handler that gives up the mutex first. */
static void unlock_it(void *mx)
{
	(void)pthread_mutex_unlock(mx);
}

/* cancel_consumes: signals that a canceled waiter swallows. */
static pthread_mutex_t swallow_lock = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t *swallow_cond[16];
static int swallow_n[16];

static void swallow(void *cv)
{
	int i;

	(void)pthread_mutex_lock(&swallow_lock);
	for (i = 0; i < 16; i++)
		if (swallow_cond[i] == cv || swallow_cond[i] == NULL) {
			swallow_cond[i] = cv;
			swallow_n[i]++;
			break;
		}
	(void)pthread_mutex_unlock(&swallow_lock);
}

static int swallowed(pthread_cond_t *cv)
{
	int i, r = 0;

	(void)pthread_mutex_lock(&swallow_lock);
	for (i = 0; i < 16; i++)
		if (swallow_cond[i] == cv && swallow_n[i] > 0) {
			swallow_n[i]--;
			r = 1;
			break;
		}
	(void)pthread_mutex_unlock(&swallow_lock);
	return r;
}

/* binding_sticks: the first mutex used with each condition variable. */
static pthread_mutex_t bind_lock = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t *bind_cond[16];
static pthread_mutex_t *bind_mutex[16];

static int bound_elsewhere(pthread_cond_t *cv, pthread_mutex_t *mx)
{
	int i, r = 0;

	(void)pthread_mutex_lock(&bind_lock);
	for (i = 0; i < 16; i++) {
		if (bind_cond[i] == cv) {
			r = bind_mutex[i] != mx;
			break;
		}
		if (bind_cond[i] == NULL) {
			bind_cond[i] = cv;
			bind_mutex[i] = mx;
			break;
		}
	}
	(void)pthread_mutex_unlock(&bind_lock);
	return r;
}

/*
 * The common path of the three waits: k is the clock abstime is on (-1
 * for an untimed wait; CLOCK_REALTIME for timedwait, whose real clock is
 * the condition variable's), how is 0, 1 or 2 for wait, timedwait,
 * clockwait.
 */
static int faulty(const char *m, int how, pthread_cond_t *cv, pthread_mutex_t *mx, clockid_t k,
		  const struct timespec *abstime)
{
	struct timespec t;
	int r, old;

	if (IS("eperm_missing") || IS("owner_dead_hidden") || IS("notrecoverable_hidden") || IS("returns_unlocked") ||
	    IS("timeout_unlocked")) {
		r = how == 0 ? real_wait(cv, mx) : how == 1 ? real_timed(cv, mx, abstime) : real_clock(cv, mx, k, abstime);
		if (IS("eperm_missing") && r == EPERM)
			return 0;	/* no ownership check: a "spurious" return */
		if (IS("owner_dead_hidden") && r == EOWNERDEAD)
			return 0;
		if (IS("notrecoverable_hidden") && r == ENOTRECOVERABLE)
			return 0;
		if (IS("returns_unlocked") && r == 0)
			(void)pthread_mutex_unlock(mx);
		if (IS("timeout_unlocked") && r == ETIMEDOUT)
			(void)pthread_mutex_unlock(mx);
		return r;
	}
	if (IS("not_atomic") && how < 2) {
		/* Release, let others run, then start waiting: a wakeup can fall in the gap. */
		(void)pthread_mutex_unlock(mx);
		sleep_ms(200);
		(void)pthread_mutex_lock(mx);
		return how == 0 ? real_wait(cv, mx) : real_timed(cv, mx, abstime);
	}
	if (IS("early_timeout") && how == 1)
		return ETIMEDOUT;
	if (IS("no_timeout") && how == 1)
		return real_wait(cv, mx);
	if (IS("default_monotonic") && how == 1) {
		/* abstime read as CLOCK_MONOTONIC whatever the clock attribute. */
		t = convert(abstime, CLOCK_MONOTONIC, CLOCK_REALTIME);
		return real_timed(cv, mx, &t);
	}
	if (IS("clock_ignored") && how == 1 && abstime->tv_nsec >= 0 && abstime->tv_nsec < 1000000000L &&
	    passed(CLOCK_REALTIME, abstime))
		return ETIMEDOUT;	/* abstime read as CLOCK_REALTIME */
	if (IS("timed_holds") && how == 1) {
		/* Waits for the timeout without ever releasing the mutex. */
		while (!passed(CLOCK_REALTIME, abstime))
			sleep_ms(5);
		return ETIMEDOUT;
	}
	if (IS("no_einval") && how == 1 && (abstime->tv_nsec < 0 || abstime->tv_nsec >= 1000000000L)) {
		t = *abstime;
		t.tv_nsec = t.tv_nsec < 0 ? 0 : 999999999L;
		return real_timed(cv, mx, &t);
	}
	if (IS("not_cancel_point")) {
		(void)pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &old);
		r = how == 0 ? real_wait(cv, mx) : how == 1 ? real_timed(cv, mx, abstime) : real_clock(cv, mx, k, abstime);
		(void)pthread_setcancelstate(old, NULL);
		return r;
	}
	if (IS("cancel_unlocked") || IS("cancel_consumes")) {
		r = 0;
		pthread_cleanup_push(IS("cancel_unlocked") ? unlock_it : swallow, IS("cancel_unlocked") ? (void *)mx : (void *)cv);
		r = how == 0 ? real_wait(cv, mx) : how == 1 ? real_timed(cv, mx, abstime) : real_clock(cv, mx, k, abstime);
		pthread_cleanup_pop(0);
		return r;
	}
	if (IS("eintr") && how < 2) {
		/* A polling wait (each return is a spurious wakeup) that reports signals as EINTR. */
		struct timespec slice = { 0, 50000000L };

		if (how == 1 && passed(CLOCK_REALTIME, abstime))
			return ETIMEDOUT;
		(void)pthread_mutex_unlock(mx);
		r = nanosleep(&slice, NULL) == -1 && errno == EINTR ? EINTR : 0;
		(void)pthread_mutex_lock(mx);
		return r;
	}
	if (IS("binding_sticks") && bound_elsewhere(cv, mx))
		return EINVAL;
	if (how == 2) {
		if (IS("clockwait_early"))
			return ETIMEDOUT;
		if (IS("clockwait_swapped") && (k == CLOCK_REALTIME || k == CLOCK_MONOTONIC))
			return real_clock(cv, mx, k == CLOCK_REALTIME ? CLOCK_MONOTONIC : CLOCK_REALTIME, abstime);
		if (IS("clockwait_no_einval")) {
			t = *abstime;
			if (t.tv_nsec < 0 || t.tv_nsec >= 1000000000L)
				t.tv_nsec = t.tv_nsec < 0 ? 0 : 999999999L;
			if (k != CLOCK_REALTIME && k != CLOCK_MONOTONIC)
				k = CLOCK_REALTIME;
			return real_clock(cv, mx, k, &t);
		}
		if (IS("clockwait_ignores_signal")) {
			(void)pthread_mutex_unlock(mx);
			while (!passed(k, abstime))
				sleep_ms(5);
			(void)pthread_mutex_lock(mx);
			return ETIMEDOUT;
		}
	}
	return how == 0 ? real_wait(cv, mx) : how == 1 ? real_timed(cv, mx, abstime) : real_clock(cv, mx, k, abstime);
}

int pthread_cond_wait(pthread_cond_t *restrict cv, pthread_mutex_t *restrict mx)
{
	const char *m = mutant_of("pthread_cond_wait.");

	init();
	if (m == NULL)
		return real_wait(cv, mx);
	return faulty(m, 0, cv, mx, (clockid_t)-1, NULL);
}

int pthread_cond_timedwait(pthread_cond_t *restrict cv, pthread_mutex_t *restrict mx,
			   const struct timespec *restrict abstime)
{
	const char *m = mutant_of("pthread_cond_wait.");

	init();
	if (m == NULL)
		return real_timed(cv, mx, abstime);
	return faulty(m, 1, cv, mx, CLOCK_REALTIME, abstime);
}

int pthread_cond_clockwait(pthread_cond_t *restrict cv, pthread_mutex_t *restrict mx, clockid_t k,
			   const struct timespec *restrict abstime)
{
	const char *m = mutant_of("pthread_cond_wait.");

	init();
	if (real_clock == NULL)
		return ENOSYS;
	if (m == NULL)
		return real_clock(cv, mx, k, abstime);
	return faulty(m, 2, cv, mx, k, abstime);
}

int pthread_cond_signal(pthread_cond_t *cv)
{
	const char *m = mutant_of("pthread_cond_wait.");

	init();
	if (IS("cancel_consumes") && swallowed(cv))
		return 0;	/* spent on a waiter that was canceled */
	return real_signal(cv);
}
