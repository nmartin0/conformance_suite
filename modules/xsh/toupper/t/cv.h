/*
 * cv.h - case conversion checks over the portable characters of the POSIX
 * locale (toupper, tolower and their _l forms). Uses ct.h for the character
 * universe. Copyright (c) 2026 Nicholas Martin. All rights reserved.
 */
#ifndef CV_H
#define CV_H

#include "ct.h"

/* up: 1 for toupper, 0 for tolower. via: 0 function, 1 header form, 2 and 3 the _l forms. */
static int cv_call(int up, int via, int c, locale_t loc)
{
	int (*volatile fu)(int) = (toupper);
	int (*volatile fd)(int) = (tolower);
	int (*volatile flu)(int, locale_t) = (toupper_l);
	int (*volatile fld)(int, locale_t) = (tolower_l);
	volatile int v = c;

	switch (via) {
	case 0: return up ? fu(v) : fd(v);
	case 1: return up ? toupper(v) : tolower(v);
	case 2: return up ? flu(v, loc) : fld(v, loc);
	case 3: return up ? toupper_l(v, loc) : tolower_l(v, loc);
	}
	return -1;
}

static const char *const cv_via[4] = { "function", "header form", "_l function", "_l header form" };

/* Each character of the source case maps to the character at the same place of the other. */
static void cv_map(int up, int via0, int via1, locale_t loc)
{
	const char *from = up ? ct_lower : ct_upper;
	const char *to = up ? ct_upper : ct_lower;
	int via, i, r;

	for (via = via0; via <= via1; via++)
		for (i = 0; i < 26; i++) {
			r = cv_call(up, via, (unsigned char)from[i], loc);
			pc_check(r == (unsigned char)to[i], "%s (%s) of character code %d returned %d, expected %d",
				up ? "toupper" : "tolower", cv_via[via], (unsigned char)from[i], r, (unsigned char)to[i]);
		}
}

/* Every examined character that is not a letter of the source case, and (eof) EOF, come back unchanged. */
static void cv_same(int up, int eof, int via0, int via1, locale_t loc)
{
	const char *from = up ? ct_lower : ct_upper;
	int via, i, r;

	for (via = via0; via <= via1; via++) {
		for (i = 0; i < ct_nuni; i++) {
			if (ct_has(from, ct_uni[i]))
				continue;
			r = cv_call(up, via, ct_uni[i], loc);
			pc_check(r == ct_uni[i], "%s (%s) of character code %d returned %d, expected it unchanged",
				up ? "toupper" : "tolower", cv_via[via], ct_uni[i], r);
		}
		if (eof) {
			r = cv_call(up, via, EOF, loc);
			pc_check(r == EOF, "%s (%s) of EOF returned %d, expected EOF",
				up ? "toupper" : "tolower", cv_via[via], r);
		}
	}
}

#endif
