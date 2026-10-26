/*
 * ls writes the converted characters; a precision limits bytes and reads no further.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fprintf.038
 * MUTANT    fprintf.ls_prec_ignored
 * MUTANT    fprintf.ls_overread
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

/* The bytes wcrtomb() gives for w from the initial shift state (setup); their count. */
static size_t mb_of(wchar_t w, char *out)
{
	mbstate_t st;
	size_t k;

	memset(&st, 0, sizeof st);
	k = wcrtomb(out, w, &st);
	pc_check(k != (size_t)-1 && k > 0 && k <= MB_LEN_MAX, "wcrtomb(%#lx) failed: %s", (unsigned long)w,
	    pc_errname(errno));
	return k;
}

/*
 * n bytes at the end of an accessible page that is followed by a page with
 * PROT_NONE (setup): reading past them raises a signal.
 */
static char *at_page_end(size_t n)
{
	long pg = sysconf(_SC_PAGESIZE);
	char *base;
	int fd;

	pc_check(pg > 0 && (size_t)pg >= n, "sysconf(_SC_PAGESIZE) gave %ld", pg);
	fd = open("map", O_RDWR | O_CREAT | O_TRUNC, 0600);
	pc_check(fd >= 0, "open(map) failed: %s", pc_errname(errno));
	pc_check(ftruncate(fd, 2 * (off_t)pg) == 0, "ftruncate failed: %s", pc_errname(errno));
	base = mmap(NULL, 2 * (size_t)pg, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	pc_check(base != MAP_FAILED, "mmap failed: %s", pc_errname(errno));
	pc_check(mprotect(base + pg, (size_t)pg, PROT_NONE) == 0, "mprotect failed: %s", pc_errname(errno));
	(void)close(fd);
	return base + pg - n;
}


int main(void)
{
	pc_begin("XSH.fprintf.038");
	{
		static const wchar_t abc[] = L"abc";
		char a[MB_LEN_MAX + 1], bb[MB_LEN_MAX + 1], c[MB_LEN_MAX + 1], want[16];
		wchar_t *g;

		NEED(setlocale(LC_ALL, "POSIX") != NULL, "setlocale(LC_ALL, \"POSIX\") failed");
		NEED(MB_CUR_MAX == 1, "MB_CUR_MAX is %d in the POSIX locale", (int)MB_CUR_MAX);
		(void)mb_of(L'a', a);
		(void)mb_of(L'b', bb);
		(void)mb_of(L'c', c);
		g = (wchar_t *)(void *)at_page_end(3 * sizeof(wchar_t));
		g[0] = L'a';
		g[1] = L'b';
		g[2] = L'c';
		want[0] = a[0];
		want[1] = bb[0];
		want[2] = c[0];
		want[3] = '\0';
		pc_test();
		SN(want, "%ls", abc);
		SN("", "%ls", L"");
		SN(want, "%.3ls", g);
		want[2] = '\0';
		SN(want, "%.2ls", abc);
		SN(want, "%.2ls", g);
	}
	pc_pass();
}
