/*
 * vfwprintf() makes its stream wide-oriented.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.vfwprintf.006
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <locale.h>
#include <signal.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <wchar.h>

/* After pc_test(): a step that is not under test gives UNRESOLVED, not FAIL. */
#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

/* A stream on path in mode (a precondition). */
static FILE *open_as(const char *path, const char *mode)
{
	FILE *f = fopen(path, mode);

	NEED(f != NULL, "fopen(%s, %s) failed: %s", path, mode, pc_errname(errno));
	return f;
}

static int vsw(wchar_t *s, size_t n, const wchar_t *fmt, ...)
{
	va_list ap;
	int r;

	va_start(ap, fmt);
	r = vswprintf(s, n, fmt, ap);
	va_end(ap);
	return r;
}

static int vfw(FILE *f, const wchar_t *fmt, ...)
{
	va_list ap;
	int r;

	va_start(ap, fmt);
	r = vfwprintf(f, fmt, ap);
	va_end(ap);
	return r;
}

static int vw(const wchar_t *fmt, ...)
{
	va_list ap;
	int r;

	va_start(ap, fmt);
	r = vwprintf(fmt, ap);
	va_end(ap);
	return r;
}

#define CALL_S(b, ...) vsw(b, 64, __VA_ARGS__)
#define CALL_F(g, ...) vfw(g, __VA_ARGS__)
#define CALL_W(...) vw(__VA_ARGS__)

int main(void)
{
	pc_begin("XSH.vfwprintf.006");
	{
		FILE *f;
		int r;

		pc_test();
		f = open_as("f", "w");
		r = vfw(f, L"x");
		NEED(r >= 0, "vfwprintf() failed: %s", pc_errname(errno));
		pc_check(fwide(f, 0) > 0, "the stream is not wide-oriented after vfwprintf()");
	}
	pc_pass();
}
