/*
 * ungetc.c - planted faults in ungetc(), for mutation checks (CI only).
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
#include <sys/types.h>
#include <unistd.h>

/* One stream with a byte pushed back through this shim, for the faults that need to know. */
static FILE *pend_f, *eof_f;
static int pend_c = EOF, pend_n;

static int real_ungetc_(int c, FILE *f)
{
	REAL(ungetc);
	return real_ungetc(c, f);
}

int ungetc(int c, FILE *f)
{
	const char *m = mutant_of("ungetc.");
	int r;

	if (m == NULL)
		return real_ungetc_(c, f);
	if (IS("noop"))
		return (unsigned char)c;
	if (IS("returns_eof") && c != EOF) {
		(void)real_ungetc_(c, f);
		return EOF;
	}
	if (IS("eof_accepted") && c == EOF)
		return real_ungetc_(0xff, f);
	if (IS("eof_discards") && c == EOF && pend_f == f && pend_n > 0) {
		(void)fgetc(f);	/* the pending byte is lost */
		return EOF;
	}
	if (IS("eof_clears_indicator") && c == EOF) {
		clearerr(f);
		return EOF;
	}
	if (IS("no_convert") && c != EOF) {
		(void)real_ungetc_(c, f);
		return c;
	}
	if (IS("fifo_order") && c != EOF && pend_f == f && pend_n > 0) {
		/* the second byte goes under the first */
		int first = fgetc(f);
		if (first == EOF || real_ungetc_(c, f) == EOF || real_ungetc_(first, f) == EOF)
			return EOF;
		pend_n = 2;
		return (unsigned char)c;
	}
	if (IS("keeps_eof") && c != EOF && feof(f))
		eof_f = f;
	if (IS("writes_through") && c != EOF) {
		long pos = ftell(f);
		r = real_ungetc_(c, f);
		if (pos > 0) {
			unsigned char b = (unsigned char)c;
			(void)pwrite(fileno(f), &b, 1, pos - 1);	/* into the file itself */
		}
		return r;
	}
	r = real_ungetc_(c, f);
	if (r != EOF && c != EOF) {
		if (pend_f != f)
			pend_n = 0;
		pend_f = f;
		pend_c = (unsigned char)c;
		pend_n++;
	}
	return r;
}

/* Reading a byte uses up pushback; the faults keep their books here. */
int fgetc(FILE *f)
{
	REAL(fgetc);
	const char *m = mutant_of("ungetc.");
	int c = real_fgetc(f);

	if (m != NULL && f == pend_f && pend_n > 0)
		pend_n--;
	if (f == eof_f)
		eof_f = NULL;
	return c;
}

int feof(FILE *f)
{
	REAL(feof);
	const char *m = mutant_of("ungetc.");

	if (IS("keeps_eof") && f == eof_f)
		return 1;	/* the indicator still reads as set after the pushback */
	return real_feof(f);
}

long ftell(FILE *f)
{
	REAL(ftell);
	const char *m = mutant_of("ungetc.");
	long r = real_ftell(f);

	if (r >= 0 && IS("position_kept") && f == pend_f && pend_n > 0)
		return r + pend_n;	/* not decremented by the pushback */
	if (r >= 0 && IS("position_not_restored") && f == pend_f && pend_n == 0 && pend_c != EOF)
		return r - 1;	/* still decremented after the byte was read */
	return r;
}

/* Puts the pending byte back after a positioning call that should have discarded it. */
static void keep_pending(FILE *f, int was)
{
	if (was > 0 && f == pend_f && pend_c != EOF) {
		(void)real_ungetc_(pend_c, f);
		pend_n = 1;
	}
}

int fseek(FILE *f, long off, int whence)
{
	REAL(fseek);
	const char *m = mutant_of("ungetc.");
	int was = f == pend_f ? pend_n : 0, r;

	if (IS("seekcur_noop") && whence == SEEK_CUR && off == 0 && was > 0)
		return 0;	/* a seek to where the stream is skipped, the pushback kept */
	if (IS("seekcur_from_kept") && whence == SEEK_CUR && was > 0)
		off += was;	/* counts from the position before the pushback */
	r = real_fseek(f, off, whence);
	if (r == 0 && f == pend_f)
		pend_n = 0;
	if (r == 0 && IS("survives_seek"))
		keep_pending(f, was);
	return r;
}

int fseeko(FILE *f, off_t off, int whence)
{
	REAL(fseeko);
	const char *m = mutant_of("ungetc.");
	int was = f == pend_f ? pend_n : 0, r = real_fseeko(f, off, whence);

	if (r == 0 && f == pend_f)
		pend_n = 0;
	if (r == 0 && IS("survives_fseeko"))
		keep_pending(f, was);
	return r;
}

int fsetpos(FILE *f, const fpos_t *pos)
{
	REAL(fsetpos);
	const char *m = mutant_of("ungetc.");
	int was = f == pend_f ? pend_n : 0, r = real_fsetpos(f, pos);

	if (r == 0 && f == pend_f)
		pend_n = 0;
	if (r == 0 && IS("survives_fsetpos"))
		keep_pending(f, was);
	return r;
}

void rewind(FILE *f)
{
	REAL(rewind);
	const char *m = mutant_of("ungetc.");
	int was = f == pend_f ? pend_n : 0;

	real_rewind(f);
	if (f == pend_f)
		pend_n = 0;
	if (IS("survives_rewind"))
		keep_pending(f, was);
}

int fflush(FILE *f)
{
	REAL(fflush);
	const char *m = mutant_of("ungetc.");
	int was = f != NULL && f == pend_f ? pend_n : 0, r = real_fflush(f);

	if (r == 0 && f != NULL && f == pend_f)
		pend_n = 0;
	if (r == 0 && f != NULL && IS("survives_fflush"))
		keep_pending(f, was);
	return r;
}
