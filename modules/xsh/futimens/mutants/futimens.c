/*
 * futimens.c - planted faults in futimens(), for mutation checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "<function>.<fault>" and apply only
 * while PC_PHASE is "test" (as set by pc.c in this process or a parent).
 */
#define _GNU_SOURCE 1
#include <dirent.h>
#include <dlfcn.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
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

#include <sys/time.h>
#include <time.h>

static int fail(int e)
{
	errno = e;
	return -1;
}

static int nsec_ok(long ns)
{
	return ns == UTIME_NOW || ns == UTIME_OMIT || (ns >= 0 && ns < 1000000000L);
}

int futimens(int fd, const struct timespec times[2])
{
	REAL(futimens);
	const char *m = mutant_of("futimens.");
	struct timespec v[2];
	struct stat st;
	int r;

	if (m == NULL)
		return real_futimens(fd, times);
	if (IS("noop") || (IS("null_ignored") && times == NULL))
		return 0;
	if (IS("bad_ok") && fcntl(fd, F_GETFD) == -1)
		return 0;
	if (IS("fdcwd_as_cwd") && fd == AT_FDCWD)
		return utimensat(AT_FDCWD, ".", times, 0);	/* sets the working directory's times */
	if (times != NULL) {
		v[0] = times[0];
		v[1] = times[1];
	}
	if (IS("swaps") && times != NULL) {
		v[0] = times[1];
		v[1] = times[0];
	}
	if (IS("einval_accepted") && times != NULL && (!nsec_ok(v[0].tv_nsec) || !nsec_ok(v[1].tv_nsec)))
		return 0;
	if (IS("needs_write") && times != NULL && geteuid() != 0 && fstat(fd, &st) == 0 && !(st.st_mode & S_IWUSR))
		return fail(EPERM);
	{
		int both_now = times == NULL || (v[0].tv_nsec == UTIME_NOW && v[1].tv_nsec == UTIME_NOW);
		int both_omit = times != NULL && v[0].tv_nsec == UTIME_OMIT && v[1].tv_nsec == UTIME_OMIT;
		int other = geteuid() != 0 && fstat(fd, &st) == 0 && st.st_uid != geteuid();

		if (IS("perm_owner_only") && both_now && other)
			return fail(EACCES);	/* write access no longer enough */
		if (IS("needs_writable_fd") && both_now && other && (fcntl(fd, F_GETFL) & O_ACCMODE) == O_RDONLY)
			return fail(EACCES);	/* asks the descriptor, not the file, for write access */
		if (IS("omit_checked") && both_omit && other)
			return fail(EPERM);
		if (IS("null_unchecked") && times == NULL && other) {
			r = real_futimens(fd, NULL);
			return r == -1 && errno == EACCES ? 0 : r;	/* reports success without the access */
		}
	}
	r = real_futimens(fd, times != NULL ? v : NULL);
	if (r == -1 && IS("eperm_as_eacces") && errno == EPERM)
		errno = EACCES;
	if (r == 0 && IS("returns_one"))
		return 1;
	return r;
}
