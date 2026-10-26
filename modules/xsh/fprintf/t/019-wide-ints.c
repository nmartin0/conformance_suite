/*
 * l, ll, j, z and t select long, long long, intmax_t, size_t and ptrdiff_t.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fprintf.019
 * MUTANT    fprintf.l_truncated
 * MUTANT    fprintf.ll_truncated
 * MUTANT    fprintf.j_truncated
 * MUTANT    fprintf.z_truncated
 * MUTANT    fprintf.t_truncated
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

/* v in base 8, 10 or 16 (upper-case letters if up), '-' first if neg: the test's own conversion. */
static const char *num(uintmax_t v, int base, int up, int neg)
{
	static char bufs[8][80];
	static int k;
	const char *dg = up ? "0123456789ABCDEF" : "0123456789abcdef";
	char *b = bufs[k++ & 7], t[80];
	int n = 0, i = 0;

	do {
		t[n++] = dg[v % (unsigned)base];
		v /= (unsigned)base;
	} while (v != 0);
	if (neg)
		b[i++] = '-';
	while (n > 0)
		b[i++] = t[--n];
	b[i] = '\0';
	return b;
}

/* v in decimal. */
static const char *sdec(intmax_t v)
{
	return v < 0 ? num((uintmax_t)0 - (uintmax_t)v, 10, 0, 1) : num((uintmax_t)v, 10, 0, 0);
}


int main(void)
{
	pc_begin("XSH.fprintf.019");
	{
		pc_test();
		SN(sdec(LONG_MIN), "%ld", (long)LONG_MIN);
		SN(sdec(LONG_MAX), "%li", (long)LONG_MAX);
		SN(num(ULONG_MAX, 10, 0, 0), "%lu", (unsigned long)ULONG_MAX);
		SN(num(ULONG_MAX, 16, 0, 0), "%lx", (unsigned long)ULONG_MAX);
		SN(sdec(LLONG_MIN), "%lld", (long long)LLONG_MIN);
		SN(num(ULLONG_MAX, 8, 0, 0), "%llo", (unsigned long long)ULLONG_MAX);
		SN(num(ULLONG_MAX, 16, 1, 0), "%llX", (unsigned long long)ULLONG_MAX);
		SN(sdec(INTMAX_MIN), "%jd", (intmax_t)INTMAX_MIN);
		SN(num(UINTMAX_MAX, 10, 0, 0), "%ju", (uintmax_t)UINTMAX_MAX);
		SN(num(SIZE_MAX, 10, 0, 0), "%zu", (size_t)SIZE_MAX);
		SN(num(SIZE_MAX, 16, 0, 0), "%zx", (size_t)SIZE_MAX);
		SN(sdec(PTRDIFF_MIN), "%td", (ptrdiff_t)PTRDIFF_MIN);
		SN(sdec(PTRDIFF_MAX), "%ti", (ptrdiff_t)PTRDIFF_MAX);
	}
	pc_pass();
}
