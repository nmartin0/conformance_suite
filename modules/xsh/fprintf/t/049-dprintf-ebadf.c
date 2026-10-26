/*
 * dprintf() on a closed or read-only descriptor fails with EBADF.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fprintf.049
 * MUTANT    fprintf.dprintf_ignores_error
 * MUTANT    fprintf.dprintf_errno
 * VARIANT   fprintf.dprintf_bytewise
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
	pc_begin("XSH.fprintf.049");
	{
		int fd, rd, r, e;

		fd = open("f", O_WRONLY | O_CREAT | O_TRUNC, 0644);
		pc_check(fd >= 0, "open(f) failed: %s", pc_errname(errno));
		rd = open("f", O_RDONLY);
		pc_check(rd >= 0, "open(f, O_RDONLY) failed: %s", pc_errname(errno));
		pc_check(close(fd) == 0, "close failed");
		pc_test();
		errno = 0;
		r = dprintf(fd, F("%s"), "abc");
		e = errno;
		pc_check(r < 0, "dprintf() on a closed descriptor returned %d", r);
		pc_check(e == EBADF, "dprintf() on a closed descriptor set errno %s, expected EBADF", pc_errname(e));
		errno = 0;
		r = dprintf(rd, F("%s"), "abc");
		e = errno;
		pc_check(r < 0, "dprintf() on a read-only descriptor returned %d", r);
		pc_check(e == EBADF, "dprintf() on a read-only descriptor set errno %s, expected EBADF", pc_errname(e));
	}
	pc_pass();
}
