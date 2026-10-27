/*
 * Integer and floating conversions, flags, width, precision and length modifiers.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fwprintf.021
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <locale.h>
#include <signal.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <wchar.h>

/* After pc_test(): a step that is not under test gives UNRESOLVED, not FAIL. */
#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

/* A stream on path in mode (a precondition). */
static FILE *open_as(const char *path, const char *mode)
{
	FILE *f = fopen(path, mode);

	NEED(f != NULL, "fopen(%s, %s) failed: %s", path, mode, pc_errname(errno));
	return f;
}

/* Opens path with flags and makes it descriptor target (setup). */
static void fd_onto(const char *path, int flags, int target)
{
	int fd = open(path, flags, 0666);

	pc_check(fd >= 0, "open(%s) failed: %s", path, pc_errname(errno));
	if (fd != target) {
		if (!(dup2(fd, target) == target))
			pc_unresolved("dup2 to %d failed: %s", target, pc_errname(errno));
		pc_check(close(fd) == 0, "close failed: %s", pc_errname(errno));
	}
}

/* The file's whole contents (at most size - 1 bytes) and its length. */
static long slurp(const char *path, char *buf, size_t size)
{
	int fd = open(path, O_RDONLY);
	long n = 0, r;

	NEED(fd >= 0, "open(%s) failed: %s", path, pc_errname(errno));
	while ((size_t)n < size - 1 && (r = read(fd, buf + n, size - 1 - (size_t)n)) > 0)
		n += r;
	(void)close(fd);
	buf[n] = '\0';
	return n;
}

/* Up to 16 bytes as hex, for messages. */
static const char *hex(const void *p, size_t n)
{
	static char out[16 * 3 + 1];
	const unsigned char *b = p;
	size_t i;

	out[0] = '\0';
	for (i = 0; i < n && i < 16; i++)
		snprintf(out + 3 * i, sizeof out - 3 * i, "%02x ", b[i]);
	return out;
}

/* 1 if the wide string w holds exactly the ASCII characters of s. */
static int weq(const wchar_t *w, const char *s)
{
	size_t i;

	for (i = 0; s[i] != '\0'; i++)
		if (w[i] != (wchar_t)(unsigned char)s[i])
			return 0;
	return w[i] == L'\0';
}

/* A wide string as ASCII for messages ('?' for anything else). */
static const char *narrow(const wchar_t *w)
{
	static char bufs[4][128];
	static int k;
	char *b = bufs[k++ & 3];
	size_t i;

	for (i = 0; w[i] != L'\0' && i < 120; i++)
		b[i] = w[i] > 31 && w[i] < 127 ? (char)w[i] : '?';
	b[i] = '\0';
	return b;
}

#define CALL_S(b, ...) swprintf(b, 64, __VA_ARGS__)
#define CALL_F(g, ...) fwprintf(g, __VA_ARGS__)
#define CALL_W(...) wprintf(__VA_ARGS__)

/*
 * Output through three functions. CASE3X(want, pre, post, format, ...):
 * the call is made through the string form (into an array), the stream form
 * (on a new file) and the standard-output form (stdout is a file named "o"),
 * each time after the statement pre; each must return strlen(want), produce
 * exactly the ASCII text want, and then satisfy the condition post.
 */
static char acc[1024];
static size_t accn;
static int wn = -1;	/* target of %n */

#define CASE3X(want, pre, post, ...) do { \
	const char *w_ = (want); \
	size_t wl_ = strlen(w_); \
	wchar_t sb_[64]; \
	char fb_[256], ob_[1024]; \
	FILE *g_; \
	int r_; \
	long fn_; \
	wmemset(sb_, L'#', 64); \
	pre; \
	r_ = CALL_S(sb_, __VA_ARGS__); \
	pc_check(r_ == (int)wl_, "the string form returned %d for \"%s\", expected %d", r_, w_, (int)wl_); \
	pc_check(weq(sb_, w_), "the string form produced \"%s\", expected \"%s\"", narrow(sb_), w_); \
	pc_check(post, "the string form: condition %s failed", #post); \
	g_ = open_as("t.out", "w"); \
	pre; \
	r_ = CALL_F(g_, __VA_ARGS__); \
	pc_check(r_ == (int)wl_, "the stream form returned %d for \"%s\", expected %d", r_, w_, (int)wl_); \
	pc_check(fclose(g_) == 0, "fclose() failed: %s", pc_errname(errno)); \
	fn_ = slurp("t.out", fb_, sizeof fb_); \
	pc_check(fn_ == (long)wl_ && memcmp(fb_, w_, wl_) == 0, "the stream form wrote %ld bytes %s, expected \"%s\"", fn_, hex(fb_, (size_t)fn_), w_); \
	pc_check(post, "the stream form: condition %s failed", #post); \
	pre; \
	r_ = CALL_W(__VA_ARGS__); \
	pc_check(r_ == (int)wl_, "the standard-output form returned %d for \"%s\", expected %d", r_, w_, (int)wl_); \
	pc_check(fflush(stdout) == 0, "fflush(stdout) failed: %s", pc_errname(errno)); \
	memcpy(acc + accn, w_, wl_); \
	accn += wl_; \
	fn_ = slurp("o", ob_, sizeof ob_); \
	pc_check(fn_ == (long)accn && memcmp(ob_, acc, accn) == 0, "the standard-output form wrote %ld bytes, expected the %ld so far (last \"%s\")", fn_, (long)accn, w_); \
	pc_check(post, "the standard-output form: condition %s failed", #post); \
} while (0)
#define CASE3(want, ...) CASE3X(want, (void)0, 1, __VA_ARGS__)

int main(void)
{
	pc_begin("XSH.fwprintf.021");
	{
		fd_onto("o", O_RDWR | O_CREAT | O_TRUNC, 1);
		pc_test();
		/* integers: sign flags, width, precision, zero padding, justification */
		CASE3("-42", L"%d", -42);
		CASE3("9", L"%i", 9);
		CASE3("+5", L"%+d", 5);
		CASE3(" 5", L"% d", 5);
		CASE3("+5", L"%+ d", 5);
		CASE3("   42", L"%5d", 42);
		CASE3("42   |", L"%-5d|", 42);
		CASE3("-0042", L"%05d", -42);
		CASE3("0042", L"%.4d", 42);
		CASE3("", L"%.0d", 0);
		CASE3("  007", L"%05.3d", 7);
		CASE3("42   |", L"%-05d|", 42);
		CASE3("  042", L"%*.*d", 5, 3, 42);
		CASE3("5", L"%.*d", -1, 5);
		CASE3("005", L"%.*d", 3, 5);
		/* unsigned conversions and alternative forms */
		CASE3("3000000000", L"%u", 3000000000u);
		CASE3("10", L"%o", 8u);
		CASE3("010", L"%#o", 8u);
		CASE3("0", L"%#o", 0u);
		CASE3("ff", L"%x", 255u);
		CASE3("FF", L"%X", 255u);
		CASE3("0xff", L"%#x", 255u);
		CASE3("0XFF", L"%#X", 255u);
		CASE3("0", L"%#x", 0u);
		CASE3("00ff", L"%.4x", 255u);
		/* length modifiers */
		CASE3("44", L"%hhu", 300);
		CASE3("4464", L"%hu", 70000);
		CASE3("123456789", L"%ld", 123456789L);
		CASE3("1234567890123", L"%lld", 1234567890123LL);
		CASE3("-5", L"%jd", (intmax_t)-5);
		CASE3("7", L"%zu", (size_t)7);
		CASE3("-3", L"%td", (ptrdiff_t)-3);
		/* floating point */
		CASE3("1.500000", L"%f", 1.5);
		CASE3("2", L"%.0f", 2.0);
		CASE3("2.", L"%#.0f", 2.0);
		CASE3("-0001.50", L"%08.2f", -1.5);
		CASE3("1.23e+04", L"%.2e", 12345.0);
		CASE3("0.000000E+00", L"%E", 0.0);
		CASE3("100000", L"%g", 100000.0);
		CASE3("1e+06", L"%g", 1000000.0);
		CASE3("0.0001", L"%g", 0.0001);
		CASE3("1E-05", L"%G", 0.00001);
		CASE3("1.00000", L"%#g", 1.0);
		CASE3("1.500000", L"%Lf", (long double)1.5L);
		CASE3("1.500000", L"%lf", 1.5);
	}
	pc_pass();
}
