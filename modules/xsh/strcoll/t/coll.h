/*
 * coll.h - the POSIX locale's collation sequence (XBD 7.3.2.6) for strcoll tests.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * The single-character strings below are the portable characters in the order
 * the standard lists them (the control characters it names, then <space> to
 * <tilde>); <NUL> cannot be in a string.
 */
#ifndef COLL_H
#define COLL_H

#include "lc.h"

static const char coll_seq[] = "\a\b\t\n\v\f\r !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";

static const struct { const char *first, *second; } coll_pair[] = {
	{ "ab", "abc" }, { "az", "b" }, { "Zz", "a" }, { "A9", "Aa" }, { "Ab", "aa" }, { "", "a" }, { "a b", "a!" },
};
#define COLL_NPAIR (sizeof coll_pair / sizeof coll_pair[0])

static locale_t coll_obj;	/* (locale_t)0: use strcoll() */

static inline int coll_cmp(const char *a, const char *b)
{
	return coll_obj == (locale_t)0 ? strcoll(a, b) : strcoll_l(a, b, coll_obj);
}

static inline void coll_check_singles(const char *w)
{
	char a[2] = { 0, 0 }, b[2] = { 0, 0 };
	size_t n = sizeof coll_seq - 1, i, j;
	int f, r;

	for (i = 0; i < n; i++)
		for (j = i + 1; j < n; j++) {
			a[0] = coll_seq[i];
			b[0] = coll_seq[j];
			f = coll_cmp(a, b);
			r = coll_cmp(b, a);
			pc_check(f < 0, "%s: the character 0x%02x did not collate before 0x%02x (got %d)", w, (unsigned)(unsigned char)a[0], (unsigned)(unsigned char)b[0], f);
			pc_check(r > 0, "%s: the character 0x%02x did not collate after 0x%02x (got %d)", w, (unsigned)(unsigned char)b[0], (unsigned)(unsigned char)a[0], r);
		}
}

static inline void coll_check_pairs(const char *w)
{
	size_t i;
	int f, r;

	for (i = 0; i < COLL_NPAIR; i++) {
		f = coll_cmp(coll_pair[i].first, coll_pair[i].second);
		r = coll_cmp(coll_pair[i].second, coll_pair[i].first);
		pc_check(f < 0, "%s: pair %zu: \"%s\" did not collate before \"%s\" (got %d)", w, i, coll_pair[i].first, coll_pair[i].second, f);
		pc_check(r > 0, "%s: pair %zu: \"%s\" did not collate after \"%s\" (got %d)", w, i, coll_pair[i].second, coll_pair[i].first, r);
	}
}

#endif
