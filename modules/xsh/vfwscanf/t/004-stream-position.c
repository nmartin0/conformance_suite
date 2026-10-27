/*
 * vfwscanf() leaves the input after the converted item unread.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.vfwscanf.004
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

static int vss(const wchar_t *ws, const wchar_t *fmt, ...)
{
	va_list ap;
	int r;

	va_start(ap, fmt);
	r = vswscanf(ws, fmt, ap);
	va_end(ap);
	return r;
}

static int vsf(FILE *f, const wchar_t *fmt, ...)
{
	va_list ap;
	int r;

	va_start(ap, fmt);
	r = vfwscanf(f, fmt, ap);
	va_end(ap);
	return r;
}

static int vsw_(const wchar_t *fmt, ...)
{
	va_list ap;
	int r;

	va_start(ap, fmt);
	r = vwscanf(fmt, ap);
	va_end(ap);
	return r;
}

#define SCS vss
#define SCF vsf
#define SCW vsw_

int main(void)
{
	pc_begin("XSH.vfwscanf.004");
	{
		FILE *f;
		int i, r;
		wint_t c;

		pc_make_file("f", "10 20");
		pc_test();
		f = open_as("f", "r");
		i = 0;
		r = vsf(f, L"%d", &i);
		pc_check(r == 1 && i == 10, "vfwscanf() returned %d with i = %d, expected 1 and 10", r, i);
		c = fgetwc(f);
		pc_check(c == L' ', "the character after the item is %#lx, expected a space left unread", (unsigned long)c);
	}
	pc_pass();
}
