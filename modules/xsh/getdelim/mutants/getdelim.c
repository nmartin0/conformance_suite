/*
 * getdelim.c - planted faults in getdelim() and getline(), for mutation checks (CI only).
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

/* "signed_delim": the delimiter is compared with each byte as a plain char. */
static ssize_t signed_delim(char **lp, size_t *np, int delim, FILE *f)
{
	size_t len = 0;
	int c;

	for (;;) {
		c = fgetc(f);
		if (c == EOF)
			break;
		if (*lp == NULL || len + 2 > *np) {
			size_t nn = (len + 2) * 2;
			char *q = realloc(*lp, nn);

			if (q == NULL)
				return -1;
			*lp = q;
			*np = nn;
		}
		(*lp)[len++] = (char)c;
		if ((signed char)c == delim)
			break;
	}
	if (len == 0)
		return -1;
	(*lp)[len] = '\0';
	return (ssize_t)len;
}

/* getdelim() with the fault named m applied around the real one. */
ssize_t getdelim(char **restrict lp, size_t *restrict np, int delim, FILE *restrict f)
{
	REAL(getdelim);
	const char *m = mutant_of("getdelim.");
	struct stat st;
	size_t old_n = np != NULL ? *np : 0;
	ssize_t r;
	int e = errno, have = 0, had_err;

	if (m == NULL)
		return real_getdelim(lp, np, delim, f);
	if (IS("einval_returns_zero") && (lp == NULL || np == NULL))
		return 0;	/* the bad call is ignored */
	if (IS("eof_not_sticky") && feof(f))
		clearerr(f);
	if (IS("error_sticky") && ferror(f))
		return -1;	/* a set error indicator stops the read */
	if (IS("signed_delim") && delim > 127)
		return signed_delim(lp, np, delim, f);
	if (IS("no_atime"))
		have = fstat(fileno(f), &st) == 0;
	had_err = ferror(f);
	r = real_getdelim(lp, np, delim, f);
	if (r == -1) {
		if (!feof(f) || ferror(f)) {
			if (IS("einval_no_indicator") && errno == EINVAL && !had_err)
				clearerr(f);
			if (IS("no_error_indicator") && !had_err)
				clearerr(f);
			if (IS("errno_lost"))
				errno = e;
		}
		return r;
	}
	if (IS("n_not_set"))
		*np = old_n;	/* the new size is not reported */
	if (IS("drops_delim") && r > 0 && (unsigned char)(*lp)[r - 1] == (unsigned char)delim)
		(*lp)[--r] = '\0';
	if (IS("no_null"))
		(*lp)[r] = 'Q';	/* no terminator */
	if (IS("no_atime") && have)
		put_atime(fileno(f), &st);
	return r;
}

/* getline() with the faults that concern it. */
ssize_t getline(char **restrict lp, size_t *restrict np, FILE *restrict f)
{
	REAL(getline);
	const char *m = mutant_of("getdelim.");
	ssize_t r = real_getline(lp, np, f);

	if (m == NULL || r <= 0)
		return r;
	if (IS("getline_drops_delim") && (*lp)[r - 1] == '\n')
		(*lp)[--r] = '\0';
	if (IS("getline_strlen"))
		return (ssize_t)strlen(*lp);	/* counts up to the first null byte */
	return r;
}
