/*
 * In the POSIX locale the radix character is a period.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fprintf.033
 * MUTANT    fprintf.radix_comma
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

/*
 * b (r returned) must be mant, then ech, then a sign and at least two
 * decimal digits giving exp (either sign when exp is 0).
 */
static void echeck(const char *what, const char *b, int r, const char *mant, char ech, int exp)
{
	size_t m = strlen(mant), nd = 0;
	int ok = r >= 0 && (size_t)r == strlen(b) && strncmp(b, mant, m) == 0 && b[m] == ech;
	long v = 0;

	if (ok) {
		const char *p = b + m + 1;
		char sg = *p++;

		ok = sg == '+' || sg == '-';
		while (*p >= '0' && *p <= '9' && v < 100000) {
			v = v * 10 + (*p++ - '0');
			nd++;
		}
		ok = ok && *p == '\0' && nd >= 2 && v == (exp < 0 ? -exp : exp) &&
		    (exp == 0 || (exp < 0) == (sg == '-'));
	}
	pc_check(ok, "%s gave \"%s\" (returned %d), expected %s%c with exponent %d (two or more digits)",
	    what, show(b, strlen(b)), r, mant, ech, exp);
}

#define SE(mant, ech, exp, f, ...) do { \
	char b_[512]; \
	int r_ = snprintf(b_, sizeof b_, F(f), __VA_ARGS__); \
	echeck(f, b_, r_, mant, ech, exp); \
} while (0)


int main(void)
{
	pc_begin("XSH.fprintf.033");
	{
		char b[64];
		int pass;

		pc_test();
		for (pass = 0; pass < 2; pass++) {
			if (pass == 1)
				NEED(setlocale(LC_ALL, "POSIX") != NULL, "setlocale(LC_ALL, \"POSIX\") failed");
			SN("1.5", "%.1f", 1.5);
			SN("0.25", "%g", 0.25);
			SE("1.5", 'e', 0, "%.1e", 1.5);
			(void)snprintf(b, sizeof b, F("%.1a"), 1.5);
			pc_check(strchr(b, '.') != NULL && strchr(b, ',') == NULL, "%%.1a of 1.5 gave \"%s\"", show(b, strlen(b)));
		}
	}
	pc_pass();
}
