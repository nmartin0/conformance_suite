/*
 * ct.h - characters of the POSIX locale's LC_CTYPE classes (portable set
 * only) and helpers that check a <ctype.h> classification function against
 * them. Membership is computed from named character sets, never from codes.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 */
#ifndef CT_H
#define CT_H

#include "pc.h"
#include <ctype.h>
#include <locale.h>
#include <stdio.h>
#include <string.h>

enum { C_ALNUM, C_ALPHA, C_BLANK, C_CNTRL, C_DIGIT, C_GRAPH, C_LOWER,
       C_PRINT, C_PUNCT, C_SPACE, C_UPPER, C_XDIGIT, C_N };

static const char *const ct_cname[C_N] = { "isalnum", "isalpha", "isblank",
	"iscntrl", "isdigit", "isgraph", "islower", "isprint", "ispunct",
	"isspace", "isupper", "isxdigit" };

static int (*const ct_fn[C_N])(int) = { (isalnum), (isalpha), (isblank),
	(iscntrl), (isdigit), (isgraph), (islower), (isprint), (ispunct),
	(isspace), (isupper), (isxdigit) };

static int (*const ct_fnl[C_N])(int, locale_t) = { (isalnum_l), (isalpha_l),
	(isblank_l), (iscntrl_l), (isdigit_l), (isgraph_l), (islower_l),
	(isprint_l), (ispunct_l), (isspace_l), (isupper_l), (isxdigit_l) };

static const char ct_upper[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
static const char ct_lower[] = "abcdefghijklmnopqrstuvwxyz";
static const char ct_digit[] = "0123456789";
static const char ct_xhex[] = "0123456789ABCDEFabcdef";
static const char ct_punct[] = "!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~";
static const char ct_ctl[] = "\a\b\t\n\v\f\r";
static const char ct_wsp[] = "\t\n\v\f\r";

/* Every character examined: NUL, the other portable controls, space, the rest. */
static int ct_uni[1 + 7 + 1 + 10 + 26 + 26 + 32];
static int ct_nuni;

static void ct_add(const char *s)
{
	for (; *s; s++)
		ct_uni[ct_nuni++] = (unsigned char)*s;
}

/* Setup phase: selects the POSIX locale and fills ct_uni. */
static void ct_init(void)
{
	pc_check(setlocale(LC_ALL, "C") != NULL, "setlocale(LC_ALL, \"C\") failed");
	ct_nuni = 0;
	ct_uni[ct_nuni++] = 0;
	ct_add(ct_ctl);
	ct_add(" ");
	ct_add(ct_digit);
	ct_add(ct_upper);
	ct_add(ct_lower);
	ct_add(ct_punct);
	pc_check(ct_nuni == (int)(sizeof ct_uni / sizeof ct_uni[0]), "character universe has the wrong size");
}

static int ct_has(const char *s, int c)
{
	return c != 0 && strchr(s, c) != NULL;
}

/* Whether character c is in class cls according to the POSIX locale definition. */
static int ct_in(int cls, int c)
{
	int up = ct_has(ct_upper, c), lo = ct_has(ct_lower, c), dg = ct_has(ct_digit, c);
	int pu = ct_has(ct_punct, c);

	switch (cls) {
	case C_ALNUM: return up || lo || dg;
	case C_ALPHA: return up || lo;
	case C_BLANK: return c == ' ' || c == '\t';
	case C_CNTRL: return c == 0 || ct_has(ct_ctl, c);
	case C_DIGIT: return dg;
	case C_GRAPH: return up || lo || dg || pu;
	case C_LOWER: return lo;
	case C_PRINT: return up || lo || dg || pu || c == ' ';
	case C_PUNCT: return pu;
	case C_SPACE: return c == ' ' || ct_has(ct_wsp, c);
	case C_UPPER: return up;
	case C_XDIGIT: return ct_has(ct_xhex, c);
	}
	return 0;
}

/*
 * One call. via 0: the function through a pointer; 1: the name as the
 * header declares it (possibly a macro); 2 and 3: the same for the _l form.
 */
static int ct_call(int via, int cls, int c, locale_t loc)
{
	int (*volatile fp)(int) = ct_fn[cls];
	int (*volatile fl)(int, locale_t) = ct_fnl[cls];
	volatile int v = c;

	switch (via) {
	case 0: return fp(v);
	case 2: return fl(v, loc);
	case 1:
		switch (cls) {
		case C_ALNUM: return isalnum(v);
		case C_ALPHA: return isalpha(v);
		case C_BLANK: return isblank(v);
		case C_CNTRL: return iscntrl(v);
		case C_DIGIT: return isdigit(v);
		case C_GRAPH: return isgraph(v);
		case C_LOWER: return islower(v);
		case C_PRINT: return isprint(v);
		case C_PUNCT: return ispunct(v);
		case C_SPACE: return isspace(v);
		case C_UPPER: return isupper(v);
		case C_XDIGIT: return isxdigit(v);
		}
		break;
	case 3:
		switch (cls) {
		case C_ALNUM: return isalnum_l(v, loc);
		case C_ALPHA: return isalpha_l(v, loc);
		case C_BLANK: return isblank_l(v, loc);
		case C_CNTRL: return iscntrl_l(v, loc);
		case C_DIGIT: return isdigit_l(v, loc);
		case C_GRAPH: return isgraph_l(v, loc);
		case C_LOWER: return islower_l(v, loc);
		case C_PRINT: return isprint_l(v, loc);
		case C_PUNCT: return ispunct_l(v, loc);
		case C_SPACE: return isspace_l(v, loc);
		case C_UPPER: return isupper_l(v, loc);
		case C_XDIGIT: return isxdigit_l(v, loc);
		}
		break;
	}
	return -1;
}

static const char *const ct_vianame[4] = { "function", "header form", "_l function", "_l header form" };

/* Checks every examined character with membership `inside` (1: members, 0: non-members). */
static void ct_scan(int cls, int inside, int via0, int via1, locale_t loc)
{
	int via, i, r;

	for (via = via0; via <= via1; via++)
		for (i = 0; i < ct_nuni; i++) {
			if (ct_in(cls, ct_uni[i]) != inside)
				continue;
			r = ct_call(via, cls, ct_uni[i], loc) != 0;
			pc_check(r == inside, "%s (%s) of character code %d returned %s, expected %s",
				ct_cname[cls], ct_vianame[via], ct_uni[i],
				r ? "non-zero" : "0", inside ? "non-zero" : "0");
		}
}

static void ct_eof(int cls, int via0, int via1, locale_t loc)
{
	int via;

	for (via = via0; via <= via1; via++)
		pc_check(ct_call(via, cls, EOF, loc) == 0, "%s (%s) of EOF returned non-zero",
			ct_cname[cls], ct_vianame[via]);
}

/* Setup phase: a locale object for the POSIX locale or UNRESOLVED. */
static locale_t ct_cloc(void)
{
	locale_t l = newlocale(LC_ALL_MASK, "C", (locale_t)0);

	if (l == (locale_t)0)
		pc_unresolved("newlocale() could not make a locale object for \"C\"");
	return l;
}

#endif
