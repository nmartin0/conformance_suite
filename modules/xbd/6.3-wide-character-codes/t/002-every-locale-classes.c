/*
 * In every locale the wide-character code of a portable character is the
 * value of its character constant: the codes of A-Z, a-z, 0-9, A-F, a-f
 * and the six white-space characters are in the classes every locale
 * definition gives them (upper, lower, digit, xdigit, space, blank and the
 * classes they imply).
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.6.3-wide-character-codes.002
 */
#include "pc.h"
#include "pcchars.h"
#include "pcl.h"
#include <wchar.h>
#include <wctype.h>

static void members(const char *loc, const char *set, int (*is)(wint_t), const char *cls)
{
	const char *p;

	for (p = set; *p != '\0'; p++)
		pc_check(is((wint_t)*p) != 0, "locale %s: the code of constant #%d is not in class %s", loc, (int)(p - set), cls);
}

int main(void)
{
	int k;

	pc_begin("XBD.6.3-wide-character-codes.002");
	pcl_collect();
	pc_test();
	for (k = 0; k < pcl_count; k++) {
		const char *l = pcl_name[k];

		pc_check(setlocale(LC_ALL, l) != NULL, "setlocale(\"%s\") failed after being accepted", l);
		members(l, pcc_upper, iswupper, "upper (A-Z)");
		members(l, pcc_lower, iswlower, "lower (a-z)");
		members(l, pcc_upper, iswalpha, "alpha (A-Z)");
		members(l, pcc_lower, iswalpha, "alpha (a-z)");
		members(l, pcc_digit, iswdigit, "digit");
		members(l, pcc_digit, iswxdigit, "xdigit (0-9)");
		members(l, pcc_xup, iswxdigit, "xdigit (A-F)");
		members(l, pcc_xlow, iswxdigit, "xdigit (a-f)");
		members(l, pcc_digit, iswalnum, "alnum (0-9)");
		members(l, pcc_space6, iswspace, "space");
		members(l, " \t", iswblank, "blank");
	}
	(void)setlocale(LC_ALL, "C");
	pc_pass();
}
