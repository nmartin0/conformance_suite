/*
 * fgetc.c - planted faults in fgetc(), for mutation checks (CI only).
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

/* fgetc() with the fault named m applied around the real one. */
int fgetc(FILE *f)
{
	REAL(fgetc);
	const char *m = mutant_of("fgetc.");
	struct stat st;
	int c, e = errno, have = 0;

	if (m == NULL)
		return real_fgetc(f);
	if (IS("eof_not_sticky") && feof(f))
		clearerr(f);	/* the indicator does not stop the read */
	if (IS("no_atime"))
		have = fstat(fileno(f), &st) == 0;
	c = real_fgetc(f);
	if (IS("restarts"))
		while (c == EOF && ferror(f) && errno == EINTR) {
			clearerr(f);
			c = real_fgetc(f);	/* the interrupted read is tried again */
		}
	if (c != EOF) {
		if (IS("sign_extends"))
			return (signed char)c;	/* the byte as a plain char */
		if (IS("no_advance"))
			(void)ungetc(c, f);	/* the byte is only looked at */
		if (IS("no_atime") && have)
			put_atime(fileno(f), &st);
		return c;
	}
	if (feof(f) && !ferror(f)) {
		if (IS("eof_no_indicator"))
			clearerr(f);
		if (IS("eof_as_byte"))
			return (unsigned char)EOF;	/* EOF converted like a byte */
		return c;
	}
	if (IS("errno_lost"))
		errno = e;
	if (IS("no_error_indicator"))
		clearerr(f);
	return c;
}
