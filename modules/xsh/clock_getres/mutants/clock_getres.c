/*
 * clock_getres.c - planted faults in clock_getres(), clock_gettime() and
 * clock_settime(), for mutation checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "clock_getres.<fault>" and apply only
 * while PC_PHASE is "test" (as set by pc.c in this process or a parent).
 * No fault ever sets a real clock: where a broken system would set one,
 * the fault only pretends to.
 */
#define _GNU_SOURCE 1
#include <dlfcn.h>
#include <errno.h>
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

static int fail(int e)
{
	errno = e;
	return -1;
}

/* One of the clock IDs <time.h> names (not a clock_getcpuclockid() one). */
static int named(clockid_t id)
{
	return id == CLOCK_REALTIME || id == CLOCK_MONOTONIC ||
	    id == CLOCK_PROCESS_CPUTIME_ID || id == CLOCK_THREAD_CPUTIME_ID;
}

static int is_cpu(clockid_t id)
{
	return id == CLOCK_PROCESS_CPUTIME_ID || id == CLOCK_THREAD_CPUTIME_ID || !named(id);
}

/* Whether the system knows the clock (asked without any fault). */
static int known(clockid_t id)
{
	REAL(clock_getres);
	struct timespec ts;

	return real_clock_getres(id, &ts) == 0;
}

int clock_getres(clockid_t id, struct timespec *res)
{
	REAL(clock_getres);
	const char *m = mutant_of("clock_getres.");
	int r;

	if (m == NULL)
		return real_clock_getres(id, res);
	if (IS("realtime_einval") && id == CLOCK_REALTIME)
		return fail(EINVAL);
	if (IS("monotonic_einval") && id == CLOCK_MONOTONIC)
		return fail(EINVAL);
	if (IS("null_efault") && res == NULL)
		return fail(EFAULT);
	if (IS("res_untouched") && res != NULL)
		return 0;
	if (IS("cputime_einval") && id == CLOCK_PROCESS_CPUTIME_ID)
		return fail(EINVAL);
	if (IS("thread_cputime_einval") && id == CLOCK_THREAD_CPUTIME_ID)
		return fail(EINVAL);
	if (IS("cpuclockid_einval") && !named(id))
		return fail(EINVAL);
	r = real_clock_getres(id, res);
	if (IS("res_zero") && r == 0 && res != NULL)
		res->tv_sec = res->tv_nsec = 0;
	if (IS("unknown_ok") && r == -1 && errno == EINVAL) {
		if (res != NULL) {
			res->tv_sec = 0;
			res->tv_nsec = 1;
		}
		return 0;
	}
	if (IS("unknown_enosys") && r == -1 && errno == EINVAL)
		return fail(ENOSYS);
	return r;
}

static int frozen_set;
static struct timespec frozen;

int clock_gettime(clockid_t id, struct timespec *tp)
{
	REAL(clock_gettime);
	const char *m = mutant_of("clock_getres.");
	static unsigned long calls;
	int r;

	if (m == NULL)
		return real_clock_gettime(id, tp);
	if (IS("cputime_einval") && id == CLOCK_PROCESS_CPUTIME_ID)
		return fail(EINVAL);
	if (IS("thread_cputime_einval") && id == CLOCK_THREAD_CPUTIME_ID)
		return fail(EINVAL);
	if (IS("cpuclockid_einval") && !named(id))
		return fail(EINVAL);
	if (IS("cputime_is_wall") && id == CLOCK_PROCESS_CPUTIME_ID)
		id = CLOCK_MONOTONIC;
	if (IS("thread_is_process") && id == CLOCK_THREAD_CPUTIME_ID)
		id = CLOCK_PROCESS_CPUTIME_ID;
	if (IS("foreign_is_self") && !named(id))
		id = CLOCK_THREAD_CPUTIME_ID;
	if ((IS("cputime_frozen") && is_cpu(id)) || (IS("cpuclockid_frozen") && !named(id)) ||
	    (IS("monotonic_frozen") && id == CLOCK_MONOTONIC)) {
		/* The first value read is returned for ever. */
		if (!frozen_set) {
			r = real_clock_gettime(id, &frozen);
			if (r != 0)
				return r;
			frozen_set = 1;
		}
		*tp = frozen;
		return 0;
	}
	r = real_clock_gettime(id, tp);
	if (IS("gettime_bad_nsec") && r == 0 && tp->tv_sec > 0) {
		tp->tv_sec -= 1;
		tp->tv_nsec += 1000000000L;
	}
	if (IS("monotonic_jitter") && r == 0 && id == CLOCK_MONOTONIC && ++calls % 1000 == 0) {
		/* every thousandth reading is 1 ms behind */
		if (tp->tv_nsec >= 1000000L) {
			tp->tv_nsec -= 1000000L;
		} else {
			tp->tv_sec -= 1;
			tp->tv_nsec += 999000000L;
		}
	}
	if (IS("gettime_unknown_ok") && r == -1 && errno == EINVAL) {
		tp->tv_sec = 0;
		tp->tv_nsec = 0;
		return 0;
	}
	if (IS("unknown_enosys") && r == -1 && errno == EINVAL)
		return fail(ENOSYS);
	return r;
}

int clock_settime(clockid_t id, const struct timespec *tp)
{
	REAL(clock_settime);
	const char *m = mutant_of("clock_getres.");
	int r;

	if (m == NULL)
		return real_clock_settime(id, tp);
	if (IS("settime_monotonic_ok") && id == CLOCK_MONOTONIC)
		return 0;	/* pretends to have set it */
	if (IS("settime_monotonic_enotsup") && id == CLOCK_MONOTONIC)
		return fail(ENOTSUP);
	if (IS("settime_nsec_ok") && tp->tv_nsec >= 1000000000L)
		return 0;	/* as if the excess were carried into seconds */
	if (IS("settime_nsec_neg_ok") && tp->tv_nsec < 0)
		return 0;
	if (IS("settime_unknown_ok") && !known(id))
		return 0;
	r = real_clock_settime(id, tp);
	if (IS("unknown_enosys") && r == -1 && errno == EINVAL && !known(id))
		return fail(ENOSYS);
	return r;
}
