/*
 * fflush.c - planted faults in fflush(), for mutation checks (CI only).
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

#include <signal.h>
#include <stdio.h>
#include <stdio_ext.h>
#include <time.h>

/* A stream whose last operation was input (or that can only read). */
static int input_only(FILE *f)
{
	int fl = fcntl(fileno(f), F_GETFL);

	return fl != -1 && ((fl & O_ACCMODE) == O_RDONLY || __freading(f));
}

int fflush(FILE *f)
{
	REAL(fflush);
	const char *m = mutant_of("fflush.");
	struct stat st;
	int r, e;

	if (m == NULL)
		return real_fflush(f);
	if (IS("noop") && f != NULL && !input_only(f))
		return 0;
	if (IS("null_noop") && f == NULL)
		return 0;
	if (IS("null_success_on_error") && f == NULL) {
		(void)real_fflush(NULL);
		return 0;	/* hides a failing stream */
	}
	if (IS("always_seeks") && f != NULL && input_only(f) && feof(f) && fstat(fileno(f), &st) == 0) {
		r = real_fflush(f);
		(void)lseek(fileno(f), st.st_size, SEEK_SET);	/* sets the stream's position even at end of file */
		return r;
	}
	if (IS("no_input_sync") && f != NULL && input_only(f))
		return 0;
	if (IS("keeps_pushback") && f != NULL && input_only(f)) {
		/* syncs the offset, but a pushed-back character stays to be read */
		int c = fgetc(f);
		long pos;
		if (c == EOF)
			return real_fflush(f);
		(void)ungetc(c, f);
		pos = ftell(f);
		if (pos >= 0)
			(void)lseek(fileno(f), pos, SEEK_SET);
		return 0;
	}
	if (IS("null_skips_input") && f == NULL) {
		off_t saved[256];
		int fd;
		for (fd = 0; fd < 256; fd++) {
			int fl = fcntl(fd, F_GETFL);
			saved[fd] = fl != -1 && (fl & O_ACCMODE) == O_RDONLY ? lseek(fd, 0, SEEK_CUR) : -1;
		}
		r = real_fflush(NULL);
		for (fd = 0; fd < 256; fd++)
			if (saved[fd] >= 0)
				(void)lseek(fd, saved[fd], SEEK_SET);
		return r;
	}
	if (IS("mtime_kept") && f != NULL && fstat(fileno(f), &st) == 0) {
		struct timespec v[2];
		r = real_fflush(f);
		v[0] = st.st_atim;
		v[1] = st.st_mtim;
		(void)futimens(fileno(f), v);
		return r;
	}
	r = real_fflush(f);
	e = errno;
	if (r == 0 && IS("returns_nonzero"))
		return 1;
	if (r == EOF && IS("errno_lost"))
		e = 0;
	if (r == EOF && f != NULL && IS("error_flag_clear"))
		clearerr(f);
	if (r == EOF && IS("success_on_error")) {
		if (f != NULL)
			clearerr(f);
		return 0;
	}
	errno = e;
	return r;
}
