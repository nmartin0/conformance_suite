/*
 * In a multibyte locale lc writes the whole character and ls never a partial one.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fprintf.039
 * MUTANT    fprintf.ls_partial
 * MUTANT    fprintf.lc_mb_first_byte
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
	pc_begin("XSH.fprintf.039");
	{
		static const char *const names[] = { "C.UTF-8", "C.utf8", "en_US.UTF-8", "en_US.utf8", "POSIX.UTF-8" };
		static const unsigned long cand[] = { 0xe9, 0x20ac, 0x3042, 0xfc, 0x416 };
		char mb[MB_LEN_MAX + 1], want[4 * MB_LEN_MAX + 1], tmp[MB_LEN_MAX];
		wchar_t s[4], w = 0;
		size_t i, k = 0;
		mbstate_t st;

		for (i = 0; i < sizeof names / sizeof names[0]; i++)
			if (setlocale(LC_ALL, names[i]) != NULL && MB_CUR_MAX > 1)
				break;
		if (i == sizeof names / sizeof names[0])
			pc_notinuse("no multibyte locale among the names tried");
		for (i = 0; i < sizeof cand / sizeof cand[0] && k < 2; i++) {
			memset(&st, 0, sizeof st);
			w = (wchar_t)cand[i];
			k = wcrtomb(tmp, w, &st);
			if (k == (size_t)-1)
				k = 0;
		}
		NEED(k >= 2 && k <= MB_LEN_MAX, "no wide character of two or more bytes found");
		memcpy(mb, tmp, k);
		mb[k] = '\0';
		s[0] = s[1] = s[2] = w;
		s[3] = 0;
		pc_test();
		SN(mb, "%lc", (wint_t)w);
		strcpy(want, mb);
		strcat(want, mb);
		strcat(want, mb);
		SN(want, "%ls", s);
		SN("", "%.*ls", (int)k - 1, s);
		SN(mb, "%.*ls", (int)k, s);
		SN(mb, "%.*ls", 2 * (int)k - 1, s);
		want[2 * k] = '\0';
		SN(want, "%.*ls", 2 * (int)k, s);
	}
	pc_pass();
}
