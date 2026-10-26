/*
 * putc.c - planted faults in putc(), for mutation checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "<function>.<fault>" and apply only
 * while PC_PHASE is "test" (as set by pc.c in this process or a parent).
 */
#define _GNU_SOURCE 1
#include <dlfcn.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
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

/* Times saved before a call, put back after it (the "not marked" faults). */
__attribute__((unused)) static void put_atime(int fd, const struct stat *st)
{
	struct timespec ts[2];
	int e = errno;

	ts[0] = st->st_atim;
	ts[1].tv_sec = 0;
	ts[1].tv_nsec = UTIME_OMIT;
	(void)futimens(fd, ts);
	errno = e;
}

__attribute__((unused)) static void put_mtime(int fd, struct timespec mt)
{
	struct timespec ts[2];
	int e = errno;

	ts[0].tv_sec = 0;
	ts[0].tv_nsec = UTIME_OMIT;
	ts[1] = mt;
	(void)futimens(fd, ts);
	errno = e;
}

/* putc() with the fault named m applied around the real one. */
int putc(int c, FILE *f)
{
	REAL(putc);
	const char *m = mutant_of("putc.");
	int r, e = errno;

	if (m == NULL)
		return real_putc(c, f);
	if (IS("noop"))
		return (unsigned char)c;	/* nothing is written */
	r = real_putc(c, f);
	if (r == EOF) {
		if (IS("errno_lost"))
			errno = e;
		if (IS("success_on_error"))
			return (unsigned char)c;
		return r;
	}
	if (IS("no_convert"))
		return c;	/* returns the argument, not the byte written */
	return r;
}
