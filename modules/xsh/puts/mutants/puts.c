/*
 * puts.c - planted faults in puts(), for mutation checks (CI only).
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

#define PREFIX "puts."

/*
 * "no_mtime": the stream's writes do not mark the modification time. The
 * time the file had before the first write through the faulty function
 * is put back after every successful fflush() or fclose() of that stream.
 */
static FILE *mt_f;
static struct timespec mt_saved;

static void mt_note(FILE *f)
{
	struct stat st;

	if (mt_f == NULL && fstat(fileno(f), &st) == 0) {
		mt_f = f;
		mt_saved = st.st_mtim;
	}
}

int fflush(FILE *f)
{
	REAL(fflush);
	const char *m = mutant_of(PREFIX);
	int r = real_fflush(f);

	if (r == 0 && IS("no_mtime") && mt_f != NULL && (f == mt_f || f == NULL))
		put_mtime(fileno(mt_f), mt_saved);
	return r;
}

int fclose(FILE *f)
{
	REAL(fclose);
	const char *m = mutant_of(PREFIX);
	int fd = -1, r;

	if (IS("no_mtime") && f == mt_f && mt_f != NULL) {
		fd = dup(fileno(f));
		mt_f = NULL;
	}
	r = real_fclose(f);
	if (fd >= 0) {
		if (r == 0)
			put_mtime(fd, mt_saved);
		(void)close(fd);
	}
	return r;
}

/* puts() with the fault named m applied around the real one. */
int puts(const char *s)
{
	REAL(puts);
	const char *m = mutant_of("puts.");
	int r, e = errno;

	if (m == NULL)
		return real_puts(s);
	if (IS("no_newline"))
		return fputs(s, stdout);	/* behaves as fputs() */
	if (IS("writes_nul"))
		return fputs(s, stdout) == EOF || fputc('\0', stdout) == EOF ||
		    fputc('\n', stdout) == EOF ? EOF : 0;	/* the terminator before the newline */
	if (IS("no_mtime"))
		mt_note(stdout);
	r = real_puts(s);
	if (r == EOF) {
		if (IS("errno_lost"))
			errno = e;
		if (IS("success_on_error"))
			return 0;
		return r;
	}
	if (IS("returns_eof"))
		return EOF;
	return r;
}
