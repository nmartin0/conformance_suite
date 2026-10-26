/*
 * e, g and a write infinities and NaNs in the very style f chose; F in its upper case.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fprintf.058
 * MUTANT    fprintf.e_inf_mixed
 * VARIANT   fprintf.inf_long
 * VARIANT   fprintf.nan_seq
 * READING   fprintf.styles_differ
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


int main(void)
{
	pc_begin("XSH.fprintf.058");
	{
		static const char *const lower[] = { "%f", "%e", "%g", "%a" }, *const upper[] = { "%F", "%E", "%G", "%A" };
		char b[64], ref[64], refu[64], nref[64];
		int r, k;

		pc_test();
		r = snprintf(ref, sizeof ref, F("%f"), (double)INFINITY);
		NEED(r > 0 && (strcmp(ref, "inf") == 0 || strcmp(ref, "infinity") == 0),
		    "%%f of an infinity gave \"%s\" (XSH.fprintf.031)", show(ref, strlen(ref)));
		(void)snprintf(refu, sizeof refu, F("%F"), (double)INFINITY);
		pc_check(strcmp(refu, strcmp(ref, "inf") == 0 ? "INF" : "INFINITY") == 0,
		    "%%F of an infinity gave \"%s\", %%f gave \"%s\"", show(refu, strlen(refu)), ref);
		for (k = 0; k < 4; k++) {
			r = snprintf(b, sizeof b, F(lower[k]), (double)INFINITY);
			pc_check(r > 0 && strcmp(b, ref) == 0, "%s of an infinity gave \"%s\", %%f gave \"%s\"", lower[k],
			    show(b, strlen(b)), ref);
			r = snprintf(b, sizeof b, F(lower[k]), -(double)INFINITY);
			pc_check(r > 0 && b[0] == '-' && strcmp(b + 1, ref) == 0, "%s of minus infinity gave \"%s\", %%f of plus \"%s\"",
			    lower[k], show(b, strlen(b)), ref);
			r = snprintf(b, sizeof b, F(upper[k]), (double)INFINITY);
			pc_check(r > 0 && strcmp(b, refu) == 0, "%s of an infinity gave \"%s\", %%F gave \"%s\"", upper[k],
			    show(b, strlen(b)), refu);
		}
#ifdef NAN
		(void)snprintf(nref, sizeof nref, F("%f"), (double)NAN);
		for (k = 0; k < 4; k++) {
			r = snprintf(b, sizeof b, F(lower[k]), (double)NAN);
			pc_check(r > 0 && strcmp(b, nref) == 0, "%s of a NaN gave \"%s\", %%f gave \"%s\"", lower[k],
			    show(b, strlen(b)), nref);
			r = snprintf(b, sizeof b, F(upper[k]), (double)NAN);
			pc_check(r > 0 && strlen(b) == strlen(nref) && (b[0] == '-') == (nref[0] == '-'),
			    "%s of a NaN gave \"%s\", %%f gave \"%s\"", upper[k], show(b, strlen(b)), nref);
		}
#else
		(void)nref;
#endif
	}
	pc_pass();
}
