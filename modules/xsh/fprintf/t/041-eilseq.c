/*
 * lc and ls fail with EILSEQ on a wide-character code that is not a character.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fprintf.041
 * MUTANT    fprintf.ilseq_skipped
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
	pc_begin("XSH.fprintf.041");
	{
		static const unsigned long cand[] = { 0x110000, 0xd800, 0x7fffffff, 0x100, 0xff, 0x80 };
		char b[64], tmp[MB_LEN_MAX];
		wchar_t w = 0, ws[3];
		size_t i;
		mbstate_t st;
		int r, e, found = 0;

		NEED(setlocale(LC_ALL, "POSIX") != NULL, "setlocale(LC_ALL, \"POSIX\") failed");
		for (i = 0; i < sizeof cand / sizeof cand[0] && !found; i++) {
			if (cand[i] > (unsigned long)WCHAR_MAX)
				continue;
			memset(&st, 0, sizeof st);
			w = (wchar_t)cand[i];
			errno = 0;
			found = wcrtomb(tmp, w, &st) == (size_t)-1 && errno == EILSEQ;
		}
		NEED(found, "every candidate wide-character code is a character in the POSIX locale");
		ws[0] = L'a';
		ws[1] = w;
		ws[2] = 0;
		pc_test();
		errno = 0;
		r = snprintf(b, sizeof b, F("a%lcb"), (wint_t)w);
		e = errno;
		pc_check(r < 0, "%%lc of %#lx returned %d, expected a negative value", (unsigned long)w, r);
		pc_check(e == EILSEQ, "%%lc of %#lx set errno %s, expected EILSEQ", (unsigned long)w, pc_errname(e));
		errno = 0;
		r = snprintf(b, sizeof b, F("%ls"), ws);
		e = errno;
		pc_check(r < 0, "%%ls with %#lx returned %d, expected a negative value", (unsigned long)w, r);
		pc_check(e == EILSEQ, "%%ls with %#lx set errno %s, expected EILSEQ", (unsigned long)w, pc_errname(e));
	}
	pc_pass();
}
