/*
 * Infinities and NaNs are written in one of the f or F styles, by every floating conversion.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fprintf.031
 * MUTANT    fprintf.inf_case
 * MUTANT    fprintf.F_lowercase
 * MUTANT    fprintf.nan_case
 * VARIANT   fprintf.inf_long
 * VARIANT   fprintf.nan_seq
 * VARIANT   fprintf.styles_differ
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

/* 1 if s is [-]w or [-]w(seq) with seq of letters, digits and '_' (sequence allowed if seq). */
static int nanlike(const char *s, const char *w, int seq)
{
	size_t n = strlen(w), i;

	if (*s == '-')
		s++;
	if (strncmp(s, w, n) != 0)
		return 0;
	s += n;
	if (*s == '\0')
		return 1;
	if (!seq || *s != '(' || s[strlen(s) - 1] != ')')
		return 0;
	for (i = 1; i + 1 < strlen(s); i++)
		if (!isalnum((unsigned char)s[i]) && s[i] != '_')
			return 0;
	return 1;
}

/* s is the style word, with a '-' first when neg. */
static int infword(const char *s, int neg, int up)
{
	if (neg && *s++ != '-')
		return 0;
	return strcmp(s, up ? "INF" : "inf") == 0 || strcmp(s, up ? "INFINITY" : "infinity") == 0;
}

int main(void)
{
	pc_begin("XSH.fprintf.031");
	{
		static const char *const conv[] = { "%f", "%e", "%g", "%a", "%F", "%E", "%G", "%A" };
		char b[64];
		int r, k;

		pc_test();
		for (k = 0; k < 8; k++) {
			int up = k >= 4;

			r = snprintf(b, sizeof b, F(conv[k]), (double)INFINITY);
			pc_check(r > 0 && infword(b, 0, up), "%s of an infinity gave \"%s\"", conv[k], show(b, strlen(b)));
			r = snprintf(b, sizeof b, F(conv[k]), -(double)INFINITY);
			pc_check(r > 0 && infword(b, 1, up), "%s of minus infinity gave \"%s\"", conv[k], show(b, strlen(b)));
#ifdef NAN
			r = snprintf(b, sizeof b, F(conv[k]), (double)NAN);
			pc_check(r > 0 && nanlike(b, up ? "NAN" : "nan", 1), "%s of a NaN gave \"%s\"", conv[k], show(b, strlen(b)));
#endif
		}
	}
	pc_pass();
}
