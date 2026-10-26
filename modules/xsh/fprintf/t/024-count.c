/*
 * n stores the bytes written so far, with each length modifier, and converts nothing.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fprintf.024
 * MUTANT    fprintf.n_off_by_one
 * MUTANT    fprintf.n_prints
 * MUTANT    fprintf.n_hh_as_int
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
	pc_begin("XSH.fprintf.024");
	{
		signed char hh[3] = { 0x55, 0x55, 0x55 };
		short h[3] = { 0x555, 0x555, 0x555 };
		int i0 = -1, i = -1;
		long l = -1;
		long long ll = -1;
		intmax_t j = -1;
		ptrdiff_t t = -1;

		pc_test();
		SN("abcdefghijkl7", "%nab%hhncd%hnef%nghi%lnj%llnk%jnl%tn%d", &i0, &hh[1], &h[1], &i, &l, &ll, &j, &t, 7);
		pc_check(i0 == 0, "%%n at the start stored %d", i0);
		pc_check(hh[1] == 2 && hh[0] == 0x55 && hh[2] == 0x55, "%%hhn stored %d (neighbours %d %d)", hh[1], hh[0], hh[2]);
		pc_check(h[1] == 4 && h[0] == 0x555 && h[2] == 0x555, "%%hn stored %d (neighbours %d %d)", h[1], h[0], h[2]);
		pc_check(i == 6, "%%n stored %d, expected 6", i);
		pc_check(l == 9, "%%ln stored %ld, expected 9", l);
		pc_check(ll == 10, "%%lln stored %lld, expected 10", ll);
		pc_check(j == 11, "%%jn stored %jd, expected 11", j);
		pc_check(t == 12, "%%tn stored %td, expected 12", t);
		SN("   42", "%5d%n", 42, &i);
		pc_check(i == 5, "%%n after %%5d stored %d, expected 5", i);
	}
	pc_pass();
}
