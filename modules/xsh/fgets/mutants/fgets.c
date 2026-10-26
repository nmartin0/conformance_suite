/*
 * fgets.c - planted faults in fgets(), for mutation checks (CI only).
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

/* "partial_ok": a line cut short by an error is still returned. */
static char *keeps_partial(char *s, int n, FILE *f)
{
	int i = 0, c;

	while (i < n - 1) {
		c = fgetc(f);
		if (c == EOF)
			break;
		s[i++] = (char)c;
		if (c == '\n')
			break;
	}
	if (i == 0)
		return NULL;
	s[i] = '\0';
	return s;
}

/* fgets() with the fault named m applied around the real one. */
char *fgets(char *restrict s, int n, FILE *restrict f)
{
	REAL(fgets);
	const char *m = mutant_of("fgets.");
	struct stat st;
	char *r;
	int e = errno, have = 0;

	if (m == NULL)
		return real_fgets(s, n, f);
	if (IS("partial_ok"))
		return keeps_partial(s, n, f);
	if (IS("no_null")) {
		char tmp[4096];

		r = real_fgets(tmp, n < (int)sizeof tmp ? n : (int)sizeof tmp, f);
		if (r != NULL)
			memcpy(s, tmp, strlen(tmp));	/* the bytes without their terminator */
		return r != NULL ? s : NULL;
	}
	if (IS("n_bytes"))
		n++;	/* reads n bytes, not n-1 */
	if (IS("no_atime"))
		have = fstat(fileno(f), &st) == 0;
	r = real_fgets(s, n, f);
	if (r != NULL) {
		if (IS("skips_rest") && strchr(s, '\n') == NULL) {
			int c;

			while ((c = fgetc(f)) != EOF && c != '\n')
				;	/* the rest of a long line is thrown away */
		}
		if (IS("drops_newline")) {
			size_t l = strlen(s);

			if (l > 0 && s[l - 1] == '\n')
				s[l - 1] = '\0';
		}
		if (IS("partial_null") && feof(f))
			return NULL;	/* a last line without newline is lost */
		if (IS("no_atime") && have)
			put_atime(fileno(f), &st);
		return r;
	}
	if (feof(f) && !ferror(f)) {
		if (IS("eof_clobbers"))
			s[0] = '\0';
		if (IS("eof_returns_s"))
			return s;
		return r;
	}
	if (IS("errno_lost"))
		errno = e;
	if (IS("no_error_indicator"))
		clearerr(f);
	return r;
}
