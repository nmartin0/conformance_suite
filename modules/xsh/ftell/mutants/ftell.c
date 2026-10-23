/*
 * ftell.c - planted faults in ftell() and ftello(), for mutation checks (CI only).
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

#include <stdio.h>
#include <stdio_ext.h>
#include <sys/types.h>

static off_t tell(FILE *f, int narrow)
{
	REAL(ftello);
	const char *m = mutant_of("ftell.");
	int e = errno;
	off_t r;

	if (IS("cached")) {
		static off_t last = -1;
		struct stat st;

		if (fileno(f) >= 0 && fstat(fileno(f), &st) != 0 && last >= 0)
			return last;	/* answers from the position it remembered, without the descriptor */
		return last = real_ftello(f);
	}
	if (IS("ignores_buffer") && fileno(f) >= 0) {
		r = lseek(fileno(f), 0, SEEK_CUR);	/* the descriptor's offset, not the stream's position */
		return r;
	}
	r = real_ftello(f);
	if (r >= 0 && IS("off_by_one") && __freading(f))
		r++;
	if (r >= 0 && narrow && IS("ftello_narrow"))
		r = (off_t)(int)r;
	if (r >= 0 && IS("clears_errno"))
		errno = 0;
	else if (r >= 0)
		errno = e;
	if (r == -1 && IS("espipe_zero") && errno == ESPIPE)
		return 0;
	if (r == -1 && IS("wrong_errno"))
		errno = errno == EIO ? ENXIO : EIO;
	return r;
}

long ftell(FILE *f)
{
	REAL(ftell);
	const char *m = mutant_of("ftell.");
	int e;
	long r;

	if (m == NULL)
		return real_ftell(f);
	if (IS("wrong_errno")) {
		r = real_ftell(f);	/* keep the real EOVERFLOW check */
		if (r == -1)
			errno = errno == EIO ? ENXIO : EIO;
		return r;
	}
	e = errno;
	r = (long)tell(f, 0);
	if (r >= 0 && !IS("clears_errno"))
		errno = e;
	return r;
}

off_t ftello(FILE *f)
{
	REAL(ftello);
	const char *m = mutant_of("ftell.");

	if (m == NULL)
		return real_ftello(f);
	return tell(f, 1);
}
