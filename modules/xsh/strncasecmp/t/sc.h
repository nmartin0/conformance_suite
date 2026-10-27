/*
 * sc.h - helpers for the case-insensitive string comparisons.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 */
#ifndef SC_H
#define SC_H

#include "pc.h"
#include <errno.h>
#include <locale.h>
#include <strings.h>
#include <string.h>

#define SC_SGN(x, y) ((((unsigned char)(x)) > ((unsigned char)(y))) - (((unsigned char)(x)) < ((unsigned char)(y))))
#define SC_SIGN(v) (((v) > 0) - ((v) < 0))

static const char sc_upper[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
static const char sc_lower[] = "abcdefghijklmnopqrstuvwxyz";
static const char sc_digit[] = "0123456789";
static const char sc_punct[] = "!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~";

/*
 * Comparison through volatile function pointers. kind 0: strcasecmp,
 * 1: strcasecmp_l (loc), 2: strncasecmp (n), 3: strncasecmp_l.
 */
static int sc_cmp(int kind, const char *a, const char *b, size_t n, locale_t loc)
{
	int (*volatile f0)(const char *, const char *) = strcasecmp;
	int (*volatile f1)(const char *, const char *, locale_t) = strcasecmp_l;
	int (*volatile f2)(const char *, const char *, size_t) = strncasecmp;
	int (*volatile f3)(const char *, const char *, size_t, locale_t) = strncasecmp_l;

	switch (kind) {
	case 0: return f0(a, b);
	case 1: return f1(a, b, loc);
	case 2: return f2(a, b, n);
	default: return f3(a, b, n, loc);
	}
}

/* Setup phase: selects the POSIX locale. */
static void sc_init(void)
{
	pc_check(setlocale(LC_ALL, "C") != NULL, "setlocale(LC_ALL, \"C\") failed");
}

static locale_t sc_cloc(void)
{
	locale_t l = newlocale(LC_ALL_MASK, "C", (locale_t)0);

	if (l == (locale_t)0)
		pc_unresolved("newlocale() could not make a locale object for \"C\"");
	return l;
}

/* Expect the sign of the comparison of a and b (n is used by kinds 2 and 3). */
static void sc_expect(int kind, const char *a, const char *b, size_t n, locale_t loc, int sign)
{
	int r = sc_cmp(kind, a, b, n, loc);

	pc_check(SC_SIGN(r) == sign, "comparison kind %d of \"%s\" and \"%s\" (n %lu) returned %d, expected sign %d",
		kind, a, b, (unsigned long)n, r, sign);
}

/*
 * The main battery, shared by every test of the order. nlim is 0 for the
 * whole-string forms; for the n forms it is the n used (large enough to
 * reach the end of every string below, so the n form must agree with the
 * whole-string form).
 */
static void sc_order(int kind, size_t nlim, locale_t loc)
{
	char a[2], b[2], c[3], d[3];
	int i, j, p, k;

	for (i = 0; i < 26; i++)
		for (j = 0; j < 26; j++)
			for (k = 0; k < 4; k++) {
				a[0] = (k & 1) ? sc_upper[i] : sc_lower[i];
				b[0] = (k & 2) ? sc_upper[j] : sc_lower[j];
				a[1] = b[1] = 0;
				sc_expect(kind, a, b, nlim, loc, SC_SGN(sc_lower[i], sc_lower[j]));
			}
	/* punctuation against letters: lowercase conversion decides the order */
	for (p = 0; sc_punct[p]; p++)
		for (i = 0; i < 26; i++)
			for (k = 0; k < 2; k++) {
				a[0] = sc_punct[p];
				b[0] = k ? sc_upper[i] : sc_lower[i];
				a[1] = b[1] = 0;
				sc_expect(kind, a, b, nlim, loc, SC_SGN(sc_punct[p], sc_lower[i]));
				sc_expect(kind, b, a, nlim, loc, SC_SGN(sc_lower[i], sc_punct[p]));
			}
	/* digits against letters, and between digits */
	for (p = 0; sc_digit[p]; p++)
		for (i = 0; i < 26; i++) {
			a[0] = sc_digit[p];
			b[0] = sc_upper[i];
			a[1] = b[1] = 0;
			sc_expect(kind, a, b, nlim, loc, SC_SGN(sc_digit[p], sc_lower[i]));
		}
	c[0] = 'a'; c[1] = sc_digit[1]; c[2] = 0;
	d[0] = 'A'; d[1] = sc_digit[2]; d[2] = 0;
	sc_expect(kind, c, d, nlim, loc, SC_SGN(sc_digit[1], sc_digit[2]));
	sc_expect(kind, d, c, nlim, loc, SC_SGN(sc_digit[2], sc_digit[1]));
	/* the first difference decides, whatever follows */
	sc_expect(kind, "abz", "ACA", nlim, loc, SC_SGN('b', 'c'));
	sc_expect(kind, "ACA", "abz", nlim, loc, SC_SGN('c', 'b'));
}

#endif
