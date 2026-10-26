/*
 * fsetpos.c - planted faults in fsetpos(), for mutation checks (CI only).
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
 * A stream's indicators, set or cleared behind the C library's back to
 * plant a fault. Both reference C libraries keep them in the first word of
 * the FILE object, with the same bits: glibc's _flags (_IO_EOF_SEEN 0x10,
 * _IO_ERR_SEEN 0x20) and musl's flags (F_EOF 16, F_ERR 32).
 */
#define PC_EOF 0x10u
#define PC_ERR 0x20u

static void set_bits(FILE *f, unsigned b)
{
	*(unsigned *)(void *)f |= b;
}

static void clear_bits(FILE *f, unsigned b)
{
	*(unsigned *)(void *)f &= ~b;
}

/* The bits of b that are set on f. */
static unsigned get_bits(FILE *f, unsigned b)
{
	return *(unsigned *)(void *)f & b;
}

/* keeps_pushback: the last byte pushed back on a stream, in any phase. */
static FILE *pushed_f;
static int pushed_c;

int ungetc(int c, FILE *f)
{
	REAL(ungetc);
	int r = real_ungetc(c, f);

	if (r != EOF) {
		pushed_f = f;
		pushed_c = r;
	}
	return r;
}

int fsetpos(FILE *f, const fpos_t *pos)
{
	REAL(fsetpos);
	REAL(ungetc);
	const char *m = mutant_of("fsetpos.");
	struct stat st0;
	unsigned bits;
	int e = errno, r, have = 0;

	if (m == NULL)
		return real_fsetpos(f, pos);
	if (IS("noop"))
		return 0;	/* reports success without moving */
	if (IS("off_by_one")) {
		fpos_t p = *pos;
		long long off;

		/* both reference C libraries keep the offset in the first 8 bytes (LP64: glibc __pos, musl __lldata) */
		memcpy(&off, &p, sizeof off);
		off++;
		memcpy(&p, &off, sizeof off);
		return real_fsetpos(f, &p);
	}
	if (IS("drops_pending"))
		__fpurge(f);	/* buffered output thrown away instead of written */
	if (IS("old_times") && fileno(f) >= 0)
		have = fstat(fileno(f), &st0) == 0;
	bits = get_bits(f, PC_EOF);
	r = real_fsetpos(f, pos);
	if (r == 0 && IS("keeps_eof") && bits)
		set_bits(f, PC_EOF);
	if (r == 0 && IS("keeps_pushback") && pushed_f == f) {
		(void)real_ungetc(pushed_c, f);	/* the pushback survives the repositioning */
		pushed_f = NULL;
	}
	if (r == 0 && IS("old_times") && have) {
		struct timespec ts[2];

		ts[0] = st0.st_atim;
		ts[1] = st0.st_mtim;
		(void)futimens(fileno(f), ts);	/* the data reaches the file without the times being marked */
	}
	if (r == 0 && IS("clears_errno"))
		errno = 0;
	else if (r == 0)
		errno = e;
	if (r != 0 && IS("wrong_errno"))
		errno = errno == EIO ? ENXIO : EIO;
	if (r != 0 && IS("ok_on_error"))
		return 0;
	if (r != 0 && IS("error_cleared"))
		clearerr(f);
	return r;
}
