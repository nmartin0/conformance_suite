/*
 * fgetpos.c - planted faults in fgetpos(), for mutation checks (CI only).
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
#include <stdio_ext.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <wchar.h>

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



/*
 * A position value for another place in the stream, made only with
 * standard calls: the stream is moved there, its position taken, and
 * the stream put back where it was. Returns 0 on success.
 */
static int pos_moved(FILE *f, fpos_t *out, off_t to, int whence)
{
	REAL(fgetpos);
	REAL(fsetpos);
	fpos_t orig;
	int e = errno, ok;

	if (real_fgetpos(f, &orig) != 0)
		return -1;
	ok = fseeko(f, to, whence) == 0 && real_fgetpos(f, out) == 0;
	(void)real_fsetpos(f, &orig);
	errno = e;
	return ok ? 0 : -1;
}
int fgetpos(FILE *restrict f, fpos_t *restrict pos)
{
	REAL(fgetpos);
	const char *m = mutant_of("fgetpos.");
	static FILE *last_f;
	static fpos_t last;
	int e = errno, r, fd;

	if (m == NULL)
		return real_fgetpos(f, pos);
	if (IS("cached") && f == last_f && (fd = fileno(f)) >= 0 && fcntl(fd, F_GETFD) == -1) {
		*pos = last;	/* answers from the position it saw last */
		errno = e;
		return 0;
	}
	r = real_fgetpos(f, pos);
	if (r == 0) {
		last_f = f;
		last = *pos;
	}
	if (r == 0 && IS("ignores_buffer") && (fd = fileno(f)) >= 0) {
		off_t at = lseek(fd, 0, SEEK_CUR);	/* the descriptor's offset, not the stream's */

		if (at != -1)
			(void)pos_moved(f, pos, at, SEEK_SET);
	}
	if (r == 0 && IS("off_by_one"))
		(void)pos_moved(f, pos, 1, SEEK_CUR);
	if (r == 0 && IS("clears_errno"))
		errno = 0;
	else if (r == 0)
		errno = e;
	if (r != 0 && IS("espipe_zero") && errno == ESPIPE) {
		FILE *z = tmpfile();	/* a position value for the start of a stream */

		if (z != NULL) {
			(void)real_fgetpos(z, pos);
			(void)fclose(z);
		}
		errno = e;
		return 0;
	}
	if (r != 0 && IS("wrong_errno"))
		errno = errno == EIO ? ENXIO : EIO;
	return r;
}
