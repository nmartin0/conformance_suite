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
 * A stream's indicators as the program sees them. The library's own are
 * left alone; a fault is planted by keeping an indicator the library does
 * not report, or by hiding one it does, in a small table that feof(),
 * ferror() and clearerr() consult. clearerr() forgets a stream's entry.
 */
#define PC_EOF 1u
#define PC_ERR 2u

static struct { FILE *f; unsigned kept, hidden; } ovl[16];

static int ovl_find(FILE *f, int make)
{
	int i;

	for (i = 0; i < 16; i++)
		if (ovl[i].f == f)
			return i;
	if (make)
		for (i = 0; i < 16; i++)
			if (ovl[i].f == NULL) {
				ovl[i].f = f;
				return i;
			}
	return -1;
}

static unsigned ovl_apply(FILE *f, unsigned seen)
{
	int i = ovl_find(f, 0);

	return i < 0 ? seen : (seen | ovl[i].kept) & ~ovl[i].hidden;
}

static void ovl_forget(FILE *f)
{
	int i = ovl_find(f, 0);

	if (i >= 0)
		ovl[i].f = NULL, ovl[i].kept = ovl[i].hidden = 0;
}

int feof(FILE *f)
{
	REAL(feof);

	return (ovl_apply(f, real_feof(f) ? PC_EOF : 0) & PC_EOF) != 0;
}

int ferror(FILE *f)
{
	REAL(ferror);

	return (ovl_apply(f, real_ferror(f) ? PC_ERR : 0) & PC_ERR) != 0;
}

/* The bits of b that f reports. */
static unsigned get_bits(FILE *f, unsigned b)
{
	return ((feof(f) ? PC_EOF : 0) | (ferror(f) ? PC_ERR : 0)) & b;
}

/* f reports the indicators in b, whatever the library says. */
static void set_bits(FILE *f, unsigned b)
{
	int i = ovl_find(f, 1);

	if (i >= 0)
		ovl[i].kept |= b, ovl[i].hidden &= ~b;
}

/* f does not report the indicators in b, whatever the library says. */
static void clear_bits(FILE *f, unsigned b)
{
	int i = ovl_find(f, 1);

	if (i >= 0)
		ovl[i].hidden |= b, ovl[i].kept &= ~b;
}

void clearerr(FILE *f)
{
	REAL(clearerr);

	ovl_forget(f);
	real_clearerr(f);
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
