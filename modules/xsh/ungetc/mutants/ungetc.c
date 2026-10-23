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

int ungetc(int c, FILE *f)
{
	REAL(ungetc);
	const char *m = mutant_of("ungetc.");
	static int held = EOF;
	int r;

	if (m == NULL)
		return real_ungetc(c, f);
	if (IS("noop"))
		return (unsigned char)c;
	if (IS("returns_eof") && c != EOF) {
		(void)real_ungetc(c, f);
		return EOF;
	}
	if (IS("eof_accepted") && c == EOF)
		return real_ungetc(0xff, f);
	if (IS("no_convert") && c != EOF) {
		(void)real_ungetc(c, f);
		return c;
	}
	if (IS("fifo_order") && c != EOF) {
		/* the second byte goes under the first */
		if (held == EOF) {
			held = (unsigned char)c;
			return real_ungetc(c, f);
		}
		r = fgetc(f);
		if (r == EOF)
			return EOF;
		if (real_ungetc(c, f) == EOF || real_ungetc(r, f) == EOF)
			return EOF;
		held = EOF;
		return (unsigned char)c;
	}
	if (IS("writes_through") && c != EOF) {
		long pos = ftell(f);
		r = real_ungetc(c, f);
		if (pos > 0) {
			unsigned char b = (unsigned char)c;
			(void)pwrite(fileno(f), &b, 1, pos - 1);	/* into the file itself */
		}
		return r;
	}
	if (IS("keeps_eof") && c != EOF) {
		int at_end = feof(f);
		r = real_ungetc(c, f);
		if (at_end && r != EOF) {
			/* the byte is pushed back but still reported as end of file */
			(void)fgetc(f);
			return r;
		}
		return r;
	}
	if (IS("position_kept") && c != EOF) {
		/* the byte is not pushed back; the read position moves instead */
		long pos = ftell(f);
		if (pos > 0 && fseek(f, pos - 1, SEEK_SET) == 0)
			return (unsigned char)c;
		return real_ungetc(c, f);
	}
	if (IS("survives_seek") && c != EOF) {
		held = (unsigned char)c;
		return real_ungetc(c, f);
	}
	return real_ungetc(c, f);
}

/* ungetc.survives_seek: a positioning call that keeps a pending byte. */
int fseek(FILE *f, long off, int whence)
{
	REAL(fseek);
	const char *m = mutant_of("ungetc.");
	int r = real_fseek(f, off, whence);

	if (IS("survives_seek") && r == 0) {
		extern int ungetc(int, FILE *);
		static __typeof__(ungetc) *ru;
		if (ru == NULL)
			ru = (__typeof__(ungetc) *)dlsym(RTLD_NEXT, "ungetc");
		(void)ru('Z', f);
	}
	return r;
}
