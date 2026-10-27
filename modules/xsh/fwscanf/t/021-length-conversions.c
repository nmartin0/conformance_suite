/*
 * Length modifiers, the integer and floating conversions and the upper-case spellings.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fwscanf.021
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

/* The ASCII string s as wide characters, in one of four rotating buffers. */
static const wchar_t *widen(const char *s)
{
	static wchar_t bufs[4][256];
	static int k;
	wchar_t *b = bufs[k++ & 3];
	size_t i;

	for (i = 0; s[i] != '\0' && i < 255; i++)
		b[i] = (wchar_t)(unsigned char)s[i];
	b[i] = L'\0';
	return b;
}

#define SCS swscanf
#define SCF fwscanf
#define SCW wscanf

/* A new file "in.txt" holding the ASCII text in, opened as a stream. */
static FILE *scan_stream(const char *in)
{
	int fd = open("in.txt", O_WRONLY | O_CREAT | O_TRUNC, 0666);
	size_t n = strlen(in);

	NEED(fd >= 0, "open(in.txt) failed: %s", pc_errname(errno));
	NEED(n == 0 || write(fd, in, n) == (ssize_t)n, "write(in.txt) failed: %s", pc_errname(errno));
	NEED(close(fd) == 0, "close(in.txt) failed: %s", pc_errname(errno));
	return open_as("in.txt", "r");
}

/* Makes standard input a new stream on a file holding the ASCII text in. */
static void scan_stdin(const char *in)
{
	FILE *f = scan_stream(in);

	(void)fclose(f);
	NEED(freopen("in.txt", "r", stdin) != NULL, "freopen(stdin) failed: %s", pc_errname(errno));
}

/*
 * SC(r, in, format, ...): the v-th scanner reads the ASCII text in, v being
 * 0 for the string form, 1 for the stream form (on the stream sf) and 2 for
 * the standard-input form; r receives the return value.
 */
#define SC(r, in, ...) do { \
	switch (v) { \
	case 0: r = SCS(widen(in), __VA_ARGS__); break; \
	case 1: sf = scan_stream(in); r = SCF(sf, __VA_ARGS__); break; \
	default: scan_stdin(in); r = SCW(__VA_ARGS__); break; \
	} \
} while (0)
static const char *const vname[3] = { "string form", "stream form", "standard-input form" };

int main(void)
{
	pc_begin("XSH.fwscanf.021");
	{
		int v, r;
		FILE *sf = NULL;
		signed char sc;
		short sh;
		long lo;
		long long ll;
		intmax_t jm;
		size_t zz;
		ptrdiff_t td;
		unsigned uo, ux, uX, uu, ui;
		float f1, f4, f5, f6, f7, f8;
		double d2;
		long double l3;

		pc_test();
		for (v = 0; v < 3; v++) {
			sc = 0; sh = 0; lo = 0; ll = 0; jm = 0; zz = 0; td = 0;
			SC(r, "-5 -300 123456789 1234567890123 -9 7 -3", L"%hhd %hd %ld %lld %jd %zu %td", &sc, &sh, &lo, &ll, &jm, &zz, &td);
			pc_check(r == 7, "%s: the length modifiers hh h l ll j z t returned %d, expected 7", vname[v], r);
			pc_check(sc == -5 && sh == -300 && lo == 123456789L && ll == 1234567890123LL && jm == -9 && zz == 7 && td == -3, "%s: the length-modified integers hold %d, %d, %ld, %lld, %lld, %lu and %ld, expected -5, -300, 123456789, 1234567890123, -9, 7 and -3", vname[v], (int)sc, (int)sh, lo, ll, (long long)jm, (unsigned long)zz, (long)td);
			uo = ux = uX = uu = ui = 0;
			SC(r, "17 ff FF 42 010", L"%o %x %X %u %i", &uo, &ux, &uX, &uu, &ui);
			pc_check(r == 5 && uo == 15 && ux == 255 && uX == 255 && uu == 42 && ui == 8, "%s: o, x, X, u and i returned %d with %u, %u, %u, %u and %u, expected 5 with 15, 255, 255, 42 and 8", vname[v], r, uo, ux, uX, uu, ui);
			f1 = f4 = f5 = f6 = f7 = f8 = 0;
			d2 = 0;
			l3 = 0;
			SC(r, "2.5 2.5 2.5 1.5E+2 2.5 0.5 1e2 0.25", L"%f %lf %Lf %E %F %G %e %g", &f1, &d2, &l3, &f4, &f5, &f6, &f7, &f8);
			pc_check(r == 8, "%s: the floating conversions returned %d, expected 8", vname[v], r);
			pc_check(f1 == 2.5f && d2 == 2.5 && l3 == 2.5L && f4 == 150.0f && f5 == 2.5f && f6 == 0.5f && f7 == 100.0f && f8 == 0.25f, "%s: the floating values are %g, %g, %Lg, %g, %g, %g, %g and %g, expected 2.5, 2.5, 2.5, 150, 2.5, 0.5, 100 and 0.25", vname[v], (double)f1, d2, l3, (double)f4, (double)f5, (double)f6, (double)f7, (double)f8);
		}
		(void)sf;
	}
	pc_pass();
}
