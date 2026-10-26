/*
 * a and A with a precision are correctly rounded (to nearest).
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fprintf.030
 * MUTANT    fprintf.a_truncates
 * VARIANT   fprintf.a_lead8
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

/* x times 2 to the power e, exactly (no overflow or underflow in the uses here). */
static double scale2(double x, int e)
{
	for (; e > 0; e--)
		x *= 2;
	for (; e < 0; e++)
		x /= 2;
	return x;
}

struct hexf {
	int neg, lead, ndig, dot, exp;
	double val, quantum;	/* the value; one unit in its last digit */
};

static int hexval(int c, int up)
{
	if (c >= '0' && c <= '9')
		return c - '0';
	if (!up && c >= 'a' && c <= 'f')
		return c - 'a' + 10;
	if (up && c >= 'A' && c <= 'F')
		return c - 'A' + 10;
	return -1;
}

/*
 * 1 if s is exactly [-]0xh[.hhh]p(+|-)d in the case given by up (0x, a-f,
 * p; or 0X, A-F, P), the exponent in decimal without leading zeros; fills h.
 */
static int parse_hex(const char *s, int up, struct hexf *h)
{
	double frac = 0, unit = 1;
	int d, esign;
	long e = 0;

	memset(h, 0, sizeof *h);
	if (*s == '-') {
		h->neg = 1;
		s++;
	}
	if (s[0] != '0' || s[1] != (up ? 'X' : 'x'))
		return 0;
	s += 2;
	if ((h->lead = hexval(*s++, up)) < 0)
		return 0;
	if (*s == '.') {
		h->dot = 1;
		s++;
		while ((d = hexval(*s, up)) >= 0) {
			unit /= 16;
			frac += d * unit;
			h->ndig++;
			s++;
		}
	}
	if (*s++ != (up ? 'P' : 'p'))
		return 0;
	if (*s != '+' && *s != '-')
		return 0;
	esign = *s++ == '-' ? -1 : 1;
	if (*s < '0' || *s > '9' || (s[0] == '0' && s[1] != '\0'))
		return 0;
	while (*s >= '0' && *s <= '9' && e < 100000)
		e = e * 10 + (*s++ - '0');
	if (*s != '\0')
		return 0;
	h->exp = (int)(esign * e);
	h->val = scale2(h->lead + frac, h->exp);
	if (h->neg)
		h->val = -h->val;
	h->quantum = scale2(unit, h->exp);
	return 1;
}


int main(void)
{
	pc_begin("XSH.fprintf.030");
	{
		static const struct { double v; int p; } c[] = {
			{ 1.0 + 0x1p-5 + 0x1p-40, 1 },
			{ 0x1.f9p+0, 1 },
			{ 0x1.234ap+3, 3 },
			{ -0x1.ffffp-2, 2 },
			{ 0x1.cp+0, 0 },
			{ 0x1.fffffffffffffp+0, 4 },
		};
		char b[128];
		struct hexf h;
		size_t i;
		int r, up;

		NEED(FLT_RADIX == 2, "FLT_RADIX is %d", FLT_RADIX);
		NEED(FLT_ROUNDS == 1, "the rounding direction is not to nearest (FLT_ROUNDS %d)", (int)FLT_ROUNDS);
		pc_test();
		for (i = 0; i < sizeof c / sizeof c[0]; i++)
			for (up = 0; up < 2; up++) {
				double err;

				r = snprintf(b, sizeof b, F(up ? "%.*A" : "%.*a"), c[i].p, c[i].v);
				pc_check(r > 0 && parse_hex(b, up, &h) && h.ndig == c[i].p && h.lead != 0,
				    "%%.%d%c of %a gave \"%s\"", c[i].p, up ? 'A' : 'a', c[i].v, show(b, strlen(b)));
				err = h.val - c[i].v;
				if (err < 0)
					err = -err;
				pc_check(err <= h.quantum / 2, "%%.%da of %a gave \"%s\", which is not a nearest value",
				    c[i].p, c[i].v, b);
			}
	}
	pc_pass();
}
