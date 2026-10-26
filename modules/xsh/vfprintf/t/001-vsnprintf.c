/*
 * vsnprintf() is snprintf() with a va_list: conversions, truncation, n zero.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.vfprintf.001
 * MUTANT    vfprintf.skips_arg
 * MUTANT    vfprintf.vsn_returns_written
 * MUTANT    vfprintf.vsn_ignores_n
 */
#include "pc.h"
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <float.h>
#include <limits.h>
#include <locale.h>
#include <math.h>
#include <signal.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <wchar.h>

/* After pc_test(): a step that is not under test gives UNRESOLVED, not FAIL. */
#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

/*
 * The format reaches the function through a volatile object, so that the
 * compiler cannot evaluate the call itself or replace it with another one.
 */
static const char *F(const char *s)
{
	static const char *volatile p;

	p = s;
	return p;
}

/* A printable copy of n bytes of s for messages (four rotating buffers). */
static const char *show(const char *s, size_t n)
{
	static const char hx[] = "0123456789abcdef";
	static char bufs[4][240];
	static int k;
	char *b = bufs[k++ & 3], *q = b;
	size_t i;

	for (i = 0; i < n && q < b + sizeof bufs[0] - 6; i++) {
		unsigned char c = (unsigned char)s[i];

		if (c >= 0x20 && c < 0x7f && c != '\\') {
			*q++ = (char)c;
		} else {
			*q++ = '\\';
			*q++ = 'x';
			*q++ = hx[c >> 4];
			*q++ = hx[c & 15];
		}
	}
	*q = '\0';
	return b;
}

/* r and the bytes at b (then a null byte) must be exactly the wn bytes of want. */
static void same(const char *what, const char *want, size_t wn, const char *b, int r)
{
	size_t got = r > 0 && r < 400 ? (size_t)r : strlen(b);

	pc_check(r >= 0 && (size_t)r == wn && memcmp(b, want, wn) == 0 && b[wn] == '\0',
	    "%s gave \"%s\" (returned %d), expected \"%s\" (%d)", what, show(b, got), r,
	    show(want, wn), (int)wn);
}

/* snprintf() into a large buffer must give exactly want. */
#define SN(want, f, ...) do { \
	char b_[512]; \
	int r_; \
	memset(b_, '#', sizeof b_); \
	r_ = snprintf(b_, sizeof b_, F(f), __VA_ARGS__); \
	same(f, want, strlen(want), b_, r_); \
} while (0)

/* The variable-argument form under test, called with a va_list built here. */
static int call_vsnprintf(char *s, size_t n, const char *f, ...)
{
	va_list ap;
	int r;

	va_start(ap, f);
	r = vsnprintf(s, n, f, ap);
	va_end(ap);
	return r;
}

#define VN(want, f, ...) do { \
	char b_[512]; \
	int r_; \
	memset(b_, '#', sizeof b_); \
	r_ = call_vsnprintf(b_, sizeof b_, F(f), __VA_ARGS__); \
	same(f, want, strlen(want), b_, r_); \
} while (0)

int main(void)
{
	pc_begin("XSH.vfprintf.001");
	{
		char b[16];
		size_t j;
		int r;

		pc_test();
		VN("ab-12-1.5-ff", "%s-%d-%.1f-%x", "ab", 12, 1.5, 255u);
		VN("   -7|x", "%5d|%c", -7, 'x');
		memset(b, '#', sizeof b);
		r = call_vsnprintf(b, 4, F("abc%s"), "def");
		pc_check(r == 6, "vsnprintf(n = 4) returned %d, expected 6", r);
		pc_check(memcmp(b, "abc", 4) == 0, "vsnprintf(n = 4) stored \"%s\"", show(b, 4));
		for (j = 4; j < sizeof b; j++)
			pc_check(b[j] == '#', "vsnprintf(n = 4) changed byte %d", (int)j);
		r = call_vsnprintf(NULL, 0, F("%s%d"), "abc", 12345);
		pc_check(r == 8, "vsnprintf(NULL, 0) returned %d, expected 8", r);
	}
	pc_pass();
}
