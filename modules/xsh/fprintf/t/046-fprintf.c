/*
 * fprintf() writes on the stream and returns the bytes transmitted, a null byte too.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fprintf.046
 * MUTANT    fprintf.fprintf_stops_at_nul
 * MUTANT    fprintf.fprintf_returns_zero
 * VARIANT   setvbuf.tiny_default
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

/* The file's contents (at most size - 1 bytes) and its length; a failure to read it is UNRESOLVED. */
static long slurp(const char *path, char *buf, size_t size)
{
	int fd = open(path, O_RDONLY);
	long n = 0, r = 0;

	if (fd < 0)
		pc_unresolved("open(%s) failed: %s", path, pc_errname(errno));
	while ((size_t)n < size - 1 && (r = read(fd, buf + n, size - 1 - (size_t)n)) > 0)
		n += r;
	if (r < 0)
		pc_unresolved("read(%s) failed: %s", path, pc_errname(errno));
	(void)close(fd);
	buf[n] = '\0';
	return n;
}

/* The file holds exactly the n bytes of want. */
static void holds(const char *path, const char *want, size_t n)
{
	char buf[256];
	long got = slurp(path, buf, sizeof buf);

	pc_check(got == (long)n && memcmp(buf, want, n) == 0, "%s holds \"%s\" (%ld bytes), expected \"%s\" (%d)",
	    path, show(buf, (size_t)got), got, show(want, n), (int)n);
}


int main(void)
{
	pc_begin("XSH.fprintf.046");
	{
		FILE *f;
		int r;

		pc_test();
		f = fopen("f", "w");
		NEED(f != NULL, "fopen(f) failed: %s", pc_errname(errno));
		r = fprintf(f, F("%s=%d\n"), "k", 42);
		pc_check(r == 5, "fprintf(%%s=%%d\\n) returned %d, expected 5", r);
		r = fprintf(f, F("a%cb"), 0);
		pc_check(r == 3, "fprintf(a%%cb) with 0 returned %d, expected 3", r);
		NEED(fclose(f) == 0, "fclose failed: %s", pc_errname(errno));
		holds("f", "k=42\na\0b", 8);
	}
	pc_pass();
}
