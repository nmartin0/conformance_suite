/*
 * ungetc.c - conforming variants of ungetc(), for the variant checks (CI
 * only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; see core/pcvariant.h. Each variant says why the
 * standard allows it.
 */
#define _GNU_SOURCE 1
#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include "pcvariant.h"

/*
 * "one_byte": a single byte of push-back. ungetc() guarantees one byte; a
 * second call on the same stream with no read or file-positioning call in
 * between may fail (XSH ungetc(), DESCRIPTION), and here it does, leaving
 * the stream as it was. A read, a positioning call, fflush() (which
 * discards the byte) or fclose() frees the slot again.
 */
#define NSTREAMS 64
static FILE *held[NSTREAMS];

static int holding(FILE *f)
{
	int i;

	for (i = 0; i < NSTREAMS; i++)
		if (held[i] == f)
			return 1;
	return 0;
}

static void hold(FILE *f)
{
	int i;

	for (i = 0; i < NSTREAMS; i++)
		if (held[i] == NULL) {
			held[i] = f;
			return;
		}
}

static void release(FILE *f)
{
	int i;

	for (i = 0; i < NSTREAMS; i++)
		if (held[i] == f || f == NULL)
			held[i] = NULL;
}

int ungetc(int c, FILE *f)
{
	const char *v = variant_of("ungetc.");
	REAL(ungetc);
	int r;

	if (!IS("one_byte"))
		return real_ungetc(c, f);
	if (c != EOF && holding(f))
		return EOF;
	r = real_ungetc(c, f);
	if (r != EOF)
		hold(f);
	return r;
}

/* Everything that ends a run of pushbacks frees the slot. */
#define FREES(ret, fn, params, args) \
	ret fn params \
	{ \
		const char *v = variant_of("ungetc."); \
		REAL(fn); \
		if (IS("one_byte")) \
			release(f); \
		return real_##fn args; \
	}

FREES(int, fgetc, (FILE *f), (f))
FREES(int, getc, (FILE *f), (f))
FREES(char *, fgets, (char *restrict s, int n, FILE *restrict f), (s, n, f))
FREES(size_t, fread, (void *restrict p, size_t s, size_t n, FILE *restrict f), (p, s, n, f))
FREES(int, fseek, (FILE *f, long off, int whence), (f, off, whence))
FREES(int, fseeko, (FILE *f, off_t off, int whence), (f, off, whence))
FREES(int, fsetpos, (FILE *f, const fpos_t *pos), (f, pos))
void rewind(FILE *f)
{
	const char *v = variant_of("ungetc.");
	REAL(rewind);

	if (IS("one_byte"))
		release(f);
	real_rewind(f);
}
FREES(int, fflush, (FILE *f), (f))
FREES(int, fclose, (FILE *f), (f))
