/*
 * sysconf.c - planted faults in sysconf(), for mutation checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "sysconf.<fault>" and apply only
 * while PC_PHASE is "test" (followed through pc.c's setenv() calls).
 */
#define _GNU_SOURCE 1
#include <dlfcn.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <unistd.h>

static char phase[16];

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

/* Calls counted for the faults that report what is "left". */
static int forks, fopens;

pid_t fork(void)
{
	static pid_t (*real)(void);
	const char *m = mutant_of("sysconf.");

	if (real == NULL)
		real = (pid_t (*)(void))dlsym(RTLD_NEXT, "fork");
	if (IS("child_max_left"))
		forks++;
	return real();
}

FILE *fopen(const char *path, const char *mode)
{
	static FILE *(*real)(const char *, const char *);
	const char *m = mutant_of("sysconf.");

	if (real == NULL)
		real = (FILE *(*)(const char *, const char *))dlsym(RTLD_NEXT, "fopen");
	if (IS("stream_max_left"))
		fopens++;
	return real(path, mode);
}

static long fail(int e)
{
	errno = e;
	return -1;
}

long sysconf(int name)
{
	static long (*real)(int);
	static long cached_open_max = -2;
	const char *m = mutant_of("sysconf.");
	struct rlimit rl;
	long r;
	int e;

	if (real == NULL)
		real = (long (*)(int))dlsym(RTLD_NEXT, "sysconf");
	if (m == NULL)
		return real(name);
	if (IS("drops_tzname") && name == _SC_TZNAME_MAX)
		return fail(EINVAL);		/* a name left unimplemented */
	if (IS("einval_unsupported") && name == _SC_TYPED_MEMORY_OBJECTS)
		return fail(EINVAL);		/* "not supported" said with EINVAL, not -1 */
	if (IS("child_max_6") && name == _SC_CHILD_MAX)
		return 6;			/* the value before IEEE Std 1003.1-2001/Cor 1-2002 */
	if (IS("tzname_3") && name == _SC_TZNAME_MAX)
		return 3;
	if (IS("pagesize_nolimit") && name == _SC_PAGESIZE)
		return -1;
	if (IS("ngroups_min") && name == _SC_NGROUPS_MAX)
		return 8;			/* {_POSIX_NGROUPS_MAX}, whatever the header says */
	if (IS("stack_min_doubled") && name == _SC_THREAD_STACK_MIN)
		return 2 * real(name) + 4096;
	if ((IS("option_off") && name == _SC_THREADS) || (IS("job_control_zero") && name == _SC_JOB_CONTROL))
		return IS("option_off") ? -1 : 0;
	if (IS("version_2008") && (name == _SC_VERSION || name == _SC_2_VERSION))
		return 200809L;
	if (IS("xopen_700") && name == _SC_XOPEN_VERSION)
		return 700;
	if (IS("option_one") && name == _SC_BARRIERS)
		return 1;
	if (IS("char_term_zero") && name == _SC_2_CHAR_TERM)
		return 0;
	if (IS("child_max_left") && name == _SC_CHILD_MAX) {
		r = real(name);
		return (r == -1 ? 100000 : r) - forks;
	}
	if (IS("stream_max_left") && name == _SC_STREAM_MAX) {
		r = real(name);
		return (r == -1 ? 1000 : r) - fopens;
	}
	if (IS("arg_max_from_stack") && name == _SC_ARG_MAX) {
		if (getrlimit(RLIMIT_STACK, &rl) == 0 && rl.rlim_cur != RLIM_INFINITY && rl.rlim_cur / 4 > 131072)
			return (long)(rl.rlim_cur / 4);
		return 131072;
	}
	if (IS("stream_max_from_nofile") && name == _SC_STREAM_MAX) {
		if (getrlimit(RLIMIT_NOFILE, &rl) == 0 && rl.rlim_cur != RLIM_INFINITY && rl.rlim_cur < LONG_MAX)
			return (long)rl.rlim_cur;
		return -1;
	}
	if (IS("open_max_cached") && name == _SC_OPEN_MAX) {
		if (cached_open_max == -2)
			cached_open_max = real(name);
		return cached_open_max;
	}
	if (IS("open_max_minus_one") && name == _SC_OPEN_MAX) {
		r = real(name);
		return r > 0 ? r - 1 : r;	/* the highest descriptor, not one more */
	}
	if (IS("invalid_returns_zero") || IS("invalid_wrong_errno")) {
		e = errno;
		r = real(name);
		if (r == -1 && errno == EINVAL) {
			if (IS("invalid_returns_zero")) {
				errno = e;
				return 0;
			}
			return fail(ENOSYS);
		}
		return r;
	}
	if (IS("page_size_zero") && (name == _SC_PAGE_SIZE || name == _SC_PAGESIZE))
		return 0;	/* a page size not yet set up */
	if (IS("clk_tck_zero") && name == _SC_CLK_TCK)
		return 0;
	if (IS("getpw_sets_errno") && name == _SC_GETPW_R_SIZE_MAX)
		return fail(ENOSYS);
	if (IS("onln_zero") && name == _SC_NPROCESSORS_ONLN)
		return 0;
	if (IS("conf_zero") && name == _SC_NPROCESSORS_CONF)
		return 0;
#ifdef _SC_NSIG
	if (IS("nsig_off_by_one") && name == _SC_NSIG)
		return real(name) - 1;	/* the highest signal number, not one more */
#endif
	if (IS("nolimit_sets_errno")) {
		e = errno;
		r = real(name);
		if (r == -1 && errno == e)
			errno = ENOSYS;	/* "no limit" reported with an error number */
		return r;
	}
	return real(name);
}
