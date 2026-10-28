/*
 * rx.h - table helpers shared by the tests of XBD chapter 9.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 */
#ifndef RX_H
#define RX_H

#include "pc.h"
#include <ctype.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* One match: so < 0 means the string must not match, else [so,eo) is pmatch[0]. */
struct rxc {
	const char *pat;
	int cf;
	const char *str;
	int ef;
	int so, eo;
};

/* One match with sub-offsets: nm elements are requested, off[] holds so/eo pairs. */
struct rxs {
	const char *pat;
	int cf;
	const char *str;
	int ef;
	int nm;
	int off[20];
};

#define RX_SENT (-7)
#define RX_N(t) (sizeof(t) / sizeof((t)[0]))

static inline void rx_cases(const struct rxc *t, size_t n)
{
	size_t i;

	for (i = 0; i < n; i++) {
		regex_t re;
		regmatch_t m[1];
		int r;

		r = regcomp(&re, t[i].pat, t[i].cf);
		pc_check(r == 0, "case %zu: regcomp(\"%s\") returned %d", i, t[i].pat, r);
		m[0].rm_so = RX_SENT;
		m[0].rm_eo = RX_SENT;
		r = regexec(&re, t[i].str, 1, m, t[i].ef);
		if (t[i].so < 0) {
			pc_check(r == REG_NOMATCH,
			    "case %zu: \"%s\" on \"%s\" should not match, got %d [%d,%d]",
			    i, t[i].pat, t[i].str, r, (int)m[0].rm_so, (int)m[0].rm_eo);
		} else {
			pc_check(r == 0, "case %zu: \"%s\" on \"%s\" should match, got %d",
			    i, t[i].pat, t[i].str, r);
			pc_check(m[0].rm_so == t[i].so && m[0].rm_eo == t[i].eo,
			    "case %zu: \"%s\" on \"%s\" matched [%d,%d], expected [%d,%d]",
			    i, t[i].pat, t[i].str, (int)m[0].rm_so, (int)m[0].rm_eo,
			    t[i].so, t[i].eo);
		}
		regfree(&re);
	}
}

static inline void rx_subs(const struct rxs *t, size_t n)
{
	size_t i;

	for (i = 0; i < n; i++) {
		regex_t re;
		regmatch_t m[10];
		int r, k;

		r = regcomp(&re, t[i].pat, t[i].cf);
		pc_check(r == 0, "case %zu: regcomp(\"%s\") returned %d", i, t[i].pat, r);
		for (k = 0; k < 10; k++) {
			m[k].rm_so = RX_SENT;
			m[k].rm_eo = RX_SENT;
		}
		r = regexec(&re, t[i].str, (size_t)t[i].nm, m, t[i].ef);
		pc_check(r == 0, "case %zu: \"%s\" on \"%s\" should match, got %d",
		    i, t[i].pat, t[i].str, r);
		for (k = 0; k < t[i].nm; k++)
			pc_check(m[k].rm_so == t[i].off[2 * k] && m[k].rm_eo == t[i].off[2 * k + 1],
			    "case %zu: \"%s\" on \"%s\": pmatch[%d] is [%d,%d], expected [%d,%d]",
			    i, t[i].pat, t[i].str, k, (int)m[k].rm_so, (int)m[k].rm_eo,
			    t[i].off[2 * k], t[i].off[2 * k + 1]);
		for (; k < 10; k++)
			pc_check(m[k].rm_so == RX_SENT && m[k].rm_eo == RX_SENT,
			    "case %zu: pmatch[%d] beyond nmatch was written", i, k);
		regfree(&re);
	}
}

/* Each character of yes alone must match the pattern, each of no must not. */
static inline void rx_set(const char *pat, int cf, const char *yes, const char *no)
{
	regex_t re;
	regmatch_t m[1];
	char s[2];
	size_t i;
	int r;

	r = regcomp(&re, pat, cf);
	pc_check(r == 0, "regcomp(\"%s\") returned %d", pat, r);
	s[1] = '\0';
	for (i = 0; yes[i] != '\0'; i++) {
		s[0] = yes[i];
		m[0].rm_so = RX_SENT;
		m[0].rm_eo = RX_SENT;
		r = regexec(&re, s, 1, m, 0);
		pc_check(r == 0 && m[0].rm_so == 0 && m[0].rm_eo == 1,
		    "\"%s\" should match the character 0x%02x", pat, (unsigned)(unsigned char)s[0]);
	}
	for (i = 0; no[i] != '\0'; i++) {
		s[0] = no[i];
		r = regexec(&re, s, 0, NULL, 0);
		pc_check(r == REG_NOMATCH, "\"%s\" should not match the character 0x%02x",
		    pat, (unsigned)(unsigned char)s[0]);
	}
	regfree(&re);
}

/* regcomp() must reject the pattern with a nonzero value. */
static inline void rx_bad(const char *pat, int cf)
{
	regex_t re;
	int r = regcomp(&re, pat, cf);

	if (r == 0)
		regfree(&re);
	pc_check(r != 0, "regcomp(\"%s\", flags 0x%x) accepted the pattern", pat, (unsigned)cf);
	pc_note("regcomp(\"%s\") returned %d", pat, r);
}

/*
 * Three error codes with the regex_t each came from: two from regcomp()
 * failures, one REG_NOMATCH from regexec().
 */
static inline void rx_getcodes(regex_t re[3], int code[3])
{
	int r;

	r = regcomp(&re[0], "\\(a\\)\\2", 0);
	if (r == 0)
		pc_unresolved("regcomp accepted a back-reference to a missing subexpression");
	code[0] = r;
	r = regcomp(&re[1], "[[.xyzzy.]]", 0);
	if (r == 0)
		pc_unresolved("regcomp accepted an unknown collating element");
	code[1] = r;
	r = regcomp(&re[2], "a", 0);
	if (r != 0)
		pc_unresolved("regcomp of a valid pattern failed");
	r = regexec(&re[2], "b", 0, NULL, 0);
	if (r == 0)
		pc_unresolved("regexec matched b against a");
	code[2] = r;
}

/* One case: compile, search, compare the whole match (so < 0: no match). */
static inline void rx_one(const char *pat, int cf, const char *str, int so, int eo)
{
	struct rxc c;

	c.pat = pat;
	c.cf = cf;
	c.str = str;
	c.ef = 0;
	c.so = so;
	c.eo = eo;
	rx_cases(&c, 1);
}

/* regcomp() must accept the pattern. */
static inline void rx_ok(const char *pat, int cf)
{
	regex_t re;
	int r = regcomp(&re, pat, cf);

	pc_check(r == 0, "regcomp(\"%s\", flags 0x%x) returned %d", pat, (unsigned)cf, r);
	regfree(&re);
}

/* Both patterns give the same verdict and the same whole-match offsets on each string. */
static inline void rx_same(const char *a, const char *b, int cf, const char *const *str)
{
	regex_t ra, rb;
	regmatch_t ma[1], mb[1];
	int r, i, xa, xb;

	r = regcomp(&ra, a, cf);
	pc_check(r == 0, "regcomp(\"%s\") returned %d", a, r);
	r = regcomp(&rb, b, cf);
	pc_check(r == 0, "regcomp(\"%s\") returned %d", b, r);
	for (i = 0; str[i] != NULL; i++) {
		ma[0].rm_so = ma[0].rm_eo = mb[0].rm_so = mb[0].rm_eo = RX_SENT;
		xa = regexec(&ra, str[i], 1, ma, 0);
		xb = regexec(&rb, str[i], 1, mb, 0);
		pc_check((xa == 0) == (xb == 0), "\"%s\" and \"%s\" differ on \"%s\" (%d, %d)",
		    a, b, str[i], xa, xb);
		if (xa == 0)
			pc_check(ma[0].rm_so == mb[0].rm_so && ma[0].rm_eo == mb[0].rm_eo,
			    "\"%s\" matched [%d,%d] but \"%s\" [%d,%d] on \"%s\"", a,
			    (int)ma[0].rm_so, (int)ma[0].rm_eo, b, (int)mb[0].rm_so,
			    (int)mb[0].rm_eo, str[i]);
	}
	regfree(&ra);
	regfree(&rb);
}

/*
 * The pattern must match the byte c and no other byte: it is searched in a
 * string holding the bytes 1 to 255 in order and again in reverse order,
 * and has to be found at the position of c both times.
 */
static inline void rx_hay(const char *pat, int cf, int c)
{
	static char up[256], down[256];
	regex_t re;
	regmatch_t m[1];
	int r, i;

	for (i = 1; i <= 255; i++) {
		up[i - 1] = (char)i;
		down[255 - i] = (char)i;
	}
	up[255] = down[255] = '\0';
	r = regcomp(&re, pat, cf);
	pc_check(r == 0, "regcomp(\"%s\", flags 0x%x) returned %d", pat, (unsigned)cf, r);
	r = regexec(&re, up, 1, m, 0);
	pc_check(r == 0 && m[0].rm_so == c - 1 && m[0].rm_eo == c,
	    "\"%s\" in the ascending bytes: result %d [%d,%d], expected byte %d only", pat, r,
	    (int)m[0].rm_so, (int)m[0].rm_eo, c);
	r = regexec(&re, down, 1, m, 0);
	pc_check(r == 0 && m[0].rm_so == 255 - c && m[0].rm_eo == 256 - c,
	    "\"%s\" in the descending bytes: result %d [%d,%d], expected byte %d only", pat, r,
	    (int)m[0].rm_so, (int)m[0].rm_eo, c);
	regfree(&re);
}

#endif
