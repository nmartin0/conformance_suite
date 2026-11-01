/*
 * exit.c - conforming variants around exit() and the stdio buffering it
 * flushes, for the variant checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; see core/pcvariant.h. Each variant says why the
 * standard allows it.
 */
#define _GNU_SOURCE 1
#include <dlfcn.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include "pcvariant.h"

/*
 * "write_through": output streams pass every byte to the file at once, as
 * if each output call were followed by fflush(), whatever setvbuf() asked
 * for. XSH 2.5 says that full and line buffering are only what is
 * intended, and that support for these characteristics is
 * implementation-defined; so a stream may hold no unwritten data at all.
 */
static void through(FILE *f)
{
	const char *v = variant_of("exit.");

	if (IS("write_through"))
		(void)fflush(f);
}

int fputs(const char *s, FILE *f)
{
	int r;
	REAL(fputs);

	r = real_fputs(s, f);
	through(f);
	return r;
}

int puts(const char *s)
{
	int r;
	REAL(puts);

	r = real_puts(s);
	through(stdout);
	return r;
}

int fputc(int c, FILE *f)
{
	int r;
	REAL(fputc);

	r = real_fputc(c, f);
	through(f);
	return r;
}

int putc(int c, FILE *f)
{
	int r;
	REAL(putc);

	r = real_putc(c, f);
	through(f);
	return r;
}

size_t fwrite(const void *p, size_t size, size_t n, FILE *f)
{
	size_t r;
	REAL(fwrite);

	r = real_fwrite(p, size, n, f);
	through(f);
	return r;
}

int vfprintf(FILE *f, const char *fmt, va_list ap)
{
	int r;
	REAL(vfprintf);

	r = real_vfprintf(f, fmt, ap);
	through(f);
	return r;
}

int fprintf(FILE *f, const char *fmt, ...)
{
	va_list ap;
	int r;
	REAL(vfprintf);

	va_start(ap, fmt);
	r = real_vfprintf(f, fmt, ap);
	va_end(ap);
	through(f);
	return r;
}

int printf(const char *fmt, ...)
{
	va_list ap;
	int r;
	REAL(vfprintf);

	va_start(ap, fmt);
	r = real_vfprintf(stdout, fmt, ap);
	va_end(ap);
	through(stdout);
	return r;
}
