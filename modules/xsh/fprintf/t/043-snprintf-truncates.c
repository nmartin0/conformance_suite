/*
 * snprintf() writes at most n-1 bytes and a null byte, and returns the full length.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fprintf.043
 * MUTANT    fprintf.sn_no_nul
 * MUTANT    fprintf.sn_returns_written
 * MUTANT    fprintf.sn_overrun
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
	pc_begin("XSH.fprintf.043");
	{
		static const size_t ns[] = { 1, 2, 4, 6, 7, 8 };
		char b[16];
		size_t i, j, k;
		int r;

		pc_test();
		for (i = 0; i < sizeof ns / sizeof ns[0]; i++) {
			memset(b, '#', sizeof b);
			r = snprintf(b, ns[i], F("abc%s"), "def");
			pc_check(r == 6, "snprintf(n = %d) returned %d, expected 6", (int)ns[i], r);
			k = ns[i] - 1 < 6 ? ns[i] - 1 : 6;
			pc_check(memcmp(b, "abcdef", k) == 0 && b[k] == '\0', "snprintf(n = %d) stored \"%s\"",
			    (int)ns[i], show(b, k + 1));
			for (j = ns[i]; j < sizeof b; j++)
				pc_check(b[j] == '#', "snprintf(n = %d) changed byte %d", (int)ns[i], (int)j);
		}
	}
	pc_pass();
}
