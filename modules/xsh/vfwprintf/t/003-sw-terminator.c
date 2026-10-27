/*
 * vswprintf() adds a terminator and counts like swprintf().
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.vfwprintf.003
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

/* 1 if the wide string w holds exactly the ASCII characters of s. */
static int weq(const wchar_t *w, const char *s)
{
	size_t i;

	for (i = 0; s[i] != '\0'; i++)
		if (w[i] != (wchar_t)(unsigned char)s[i])
			return 0;
	return w[i] == L'\0';
}

/* A wide string as ASCII for messages ('?' for anything else). */
static const char *narrow(const wchar_t *w)
{
	static char bufs[4][128];
	static int k;
	char *b = bufs[k++ & 3];
	size_t i;

	for (i = 0; w[i] != L'\0' && i < 120; i++)
		b[i] = w[i] > 31 && w[i] < 127 ? (char)w[i] : '?';
	b[i] = '\0';
	return b;
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
	pc_begin("XSH.vfwprintf.003");
	{
		wchar_t buf[16];
		int r;

		wmemset(buf, L'#', 16);
		pc_test();
		r = vsw(buf, 10, L"ab%d", 5);
		pc_check(r == 3, "vswprintf() returned %d, expected 3", r);
		pc_check(buf[0] == L'a' && buf[1] == L'b' && buf[2] == L'5' && buf[3] == L'\0', "the array holds \"%s\" then %#lx, expected \"ab5\" and a null", narrow(buf), (unsigned long)buf[3]);
		pc_check(buf[4] == L'#', "an element after the terminator was changed");
		wmemset(buf, L'#', 16);
		r = vsw(buf, 4, L"abc");
		pc_check(r == 3, "vswprintf(n 4) of three characters returned %d, expected 3", r);
		r = vsw(buf, 3, L"abc");
		pc_check(r < 0, "vswprintf(n 3) of three characters returned %d, expected a negative value", r);
	}
	pc_pass();
}
