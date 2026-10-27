/*
 * vswprintf() fails with EOVERFLOW when n or more characters are requested.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.vfwprintf.004
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
	pc_begin("XSH.vfwprintf.004");
	{
		wchar_t buf[16];
		int r, e, i, nul = 0;

		wmemset(buf, L'#', 16);
		pc_test();
		errno = 0;
		r = vsw(buf, 4, L"abcdef");
		e = errno;
		pc_check(r < 0, "vswprintf() returned %d for output longer than n, expected a negative value", r);
		pc_check(e == EOVERFLOW, "vswprintf() set errno %s, expected EOVERFLOW", pc_errname(e));
		for (i = 4; i < 16; i++)
			pc_check(buf[i] == L'#', "element %d, beyond n, was changed", i);
		for (i = 0; i < 4; i++)
			if (buf[i] == L'\0')
				nul = 1;
		pc_check(nul, "no null wide character among the first four elements");
		wmemset(buf, L'#', 16);
		r = vsw(buf, 0, L"abc");
		pc_check(r < 0, "vswprintf() with n 0 returned %d, expected a negative value", r);
		for (i = 0; i < 16; i++)
			pc_check(buf[i] == L'#', "element %d was changed with n 0", i);
	}
	pc_pass();
}
