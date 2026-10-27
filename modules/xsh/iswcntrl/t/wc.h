/*
 * wc.h - the 128 characters classified by the POSIX locale's LC_CTYPE, and
 * helpers that check a classification function against them.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 */
#ifndef WC_H
#define WC_H

#include "pc.h"
#include <errno.h>
#include <locale.h>
#include <stdlib.h>
#include <wchar.h>
#include <wctype.h>

enum { C_ALNUM, C_ALPHA, C_BLANK, C_CNTRL, C_DIGIT, C_GRAPH, C_LOWER,
       C_PRINT, C_PUNCT, C_SPACE, C_UPPER, C_XDIGIT, C_N };

static const char *const wc_cname[C_N] = { "alnum", "alpha", "blank", "cntrl",
	"digit", "graph", "lower", "print", "punct", "space", "upper", "xdigit" };

/* wc_ch[i] is the wide-character code of the character at ISO 646 position i. */
static wint_t wc_ch[128];

/* Setup phase: fills wc_ch. */
static void wc_init(void)
{
	static const wchar_t pr[] = L" !\"#$%&'()*+,-./0123456789:;<=>?@"
		L"ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
	int i;

	pc_check(sizeof pr / sizeof pr[0] == 96, "portable character table has the wrong size");
	for (i = 32; i < 127; i++)
		wc_ch[i] = (wint_t)pr[i - 32];
	/*
	 * Only the control characters of the portable character set have a
	 * value the text fixes; the other control positions (1 to 6, 14 to
	 * 31, 127) stand in as the null character, which has the same class
	 * membership as they do.
	 */
	for (i = 0; i < 128; i++)
		if (i < 32 || i == 127)
			wc_ch[i] = (wint_t)L'\0';
	wc_ch[0] = (wint_t)L'\0';
	wc_ch[7] = (wint_t)L'\a';
	wc_ch[8] = (wint_t)L'\b';
	wc_ch[9] = (wint_t)L'\t';
	wc_ch[10] = (wint_t)L'\n';
	wc_ch[11] = (wint_t)L'\v';
	wc_ch[12] = (wint_t)L'\f';
	wc_ch[13] = (wint_t)L'\r';
}

/* Whether position i is in class cls according to the POSIX locale definition. */
static int wc_in(int cls, int i)
{
	int up = i >= 65 && i <= 90, lo = i >= 97 && i <= 122, dg = i >= 48 && i <= 57;

	switch (cls) {
	case C_ALNUM: return up || lo || dg;
	case C_ALPHA: return up || lo;
	case C_BLANK: return i == 32 || i == 9;
	case C_CNTRL: return i < 32 || i == 127;
	case C_DIGIT: return dg;
	case C_GRAPH: return i > 32 && i < 127;
	case C_LOWER: return lo;
	case C_PRINT: return i >= 32 && i < 127;
	case C_PUNCT: return i > 32 && i < 127 && !up && !lo && !dg;
	case C_SPACE: return i == 32 || (i >= 9 && i <= 13);
	case C_UPPER: return up;
	case C_XDIGIT: return dg || (i >= 65 && i <= 70) || (i >= 97 && i <= 102);
	}
	return 0;
}

/*
 * Checks fn over the positions in class cls (inside = 1), outside it
 * (inside = 0) or all of them (inside = -1): the answer must be non-zero
 * when want is 1 and zero when want is 0.
 */
static void wc_chk(const char *nm, int (*fn)(wint_t), int cls, int inside, int want)
{
	int i, r;

	for (i = 0; i < 128; i++) {
		if (inside >= 0 && wc_in(cls, i) != inside)
			continue;
		r = fn(wc_ch[i]) != 0;
		pc_check(r == want, "%s() of the character at ISO 646 position %d returned %s, expected %s",
			nm, i, r ? "non-zero" : "0", want ? "non-zero" : "0");
	}
}

/* One position. */
static void wc_one(const char *nm, int (*fn)(wint_t), int idx, int want)
{
	int r = fn(wc_ch[idx]) != 0;

	pc_check(r == want, "%s() of the character at ISO 646 position %d returned %s, expected %s",
		nm, idx, r ? "non-zero" : "0", want ? "non-zero" : "0");
}

/* Setup phase: switches LC_CTYPE to an installed UTF-8 locale or ends NOTINUSE. */
static void wc_utf8(void)
{
	const char *n = pc_find_locale("UTF-8", 1);

	if (n == NULL)
		pc_notinuse("no UTF-8 locale installed");
	if (setlocale(LC_CTYPE, n) == NULL)
		pc_unresolved("setlocale(LC_CTYPE, \"%s\") failed", n);
}

static int (*const wc_isw[C_N])(wint_t) = { iswalnum, iswalpha, iswblank,
	iswcntrl, iswdigit, iswgraph, iswlower, iswprint, iswpunct, iswspace,
	iswupper, iswxdigit };

#define WC_NZ(f) (f(w) != 0)

#endif
