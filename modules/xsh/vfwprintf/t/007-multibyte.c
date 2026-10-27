/*
 * The v-forms convert and count multibyte characters like the others.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.vfwprintf.007
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

/* The file's whole contents (at most size - 1 bytes) and its length. */
static long slurp(const char *path, char *buf, size_t size)
{
	int fd = open(path, O_RDONLY);
	long n = 0, r;

	NEED(fd >= 0, "open(%s) failed: %s", path, pc_errname(errno));
	while ((size_t)n < size - 1 && (r = read(fd, buf + n, size - 1 - (size_t)n)) > 0)
		n += r;
	(void)close(fd);
	buf[n] = '\0';
	return n;
}

/* Up to 16 bytes as hex, for messages. */
static const char *hex(const void *p, size_t n)
{
	static char out[16 * 3 + 1];
	const unsigned char *b = p;
	size_t i;

	out[0] = '\0';
	for (i = 0; i < n && i < 16; i++)
		snprintf(out + 3 * i, sizeof out - 3 * i, "%02x ", b[i]);
	return out;
}

/* Setup: switches to an installed UTF-8 locale, or the test is not in use. */
static void utf8(void)
{
	const char *name = pc_find_locale("UTF-8", 1);

	if (name == NULL)
		pc_notinuse("no UTF-8 locale installed");
	if (setlocale(LC_ALL, name) == NULL)
		pc_unresolved("setlocale(%s) failed", name);
}


/* Setup: the wide character mbrtowc() gives for the two bytes 0xc3 0xa9. */
static wchar_t e_acute(void)
{
	mbstate_t st;
	wchar_t w = 0;

	memset(&st, 0, sizeof st);
	if (!(mbrtowc(&w, "\xc3\xa9", 2, &st) == 2))
		pc_unresolved("mbrtowc() does not convert the bytes c3 a9 to one character");
	return w;
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
	pc_begin("XSH.vfwprintf.007");
	{
		wchar_t w, buf[8];
		char fb[8];
		FILE *f;
		int r;
		long n;

		utf8();
		w = e_acute();
		pc_test();
		wmemset(buf, L'#', 8);
		r = vsw(buf, 8, L"%s", "\xc3\xa9");
		pc_check(r == 1 && buf[0] == w && buf[1] == L'\0', "vswprintf(%%s) of the bytes c3 a9 returned %d, expected 1 element holding the character", r);
		f = open_as("f", "w");
		r = vfw(f, L"%s", "\xc3\xa9");
		pc_check(r == 1, "vfwprintf(%%s) of the bytes c3 a9 returned %d, expected 1 (wide characters, not bytes)", r);
		pc_check(fclose(f) == 0, "fclose() failed: %s", pc_errname(errno));
		n = slurp("f", fb, sizeof fb);
		pc_check(n == 2 && memcmp(fb, "\xc3\xa9", 2) == 0, "the file holds %ld bytes %s, expected c3 a9", n, hex(fb, (size_t)n));
	}
	pc_pass();
}
