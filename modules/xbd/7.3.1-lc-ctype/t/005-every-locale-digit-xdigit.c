/*
 * In every locale the class digit holds only the ten digits and the class
 * xdigit only the ten digits and A-F and a-f: of all the valid wide-
 * character codes below 0x10000, no other one is in either class.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.7.3.1-lc-ctype.005
 */
#include "pc.h"
#include "pcchars.h"
#include "pcl.h"
#include <wchar.h>
#include <wctype.h>

int main(void)
{
	long v;
	int k;

	pc_begin("XBD.7.3.1-lc-ctype.005");
	pcc_init();
	pcl_collect();
	pc_test();
	for (k = 0; k < pcl_count; k++) {
		pc_check(setlocale(LC_ALL, pcl_name[k]) != NULL, "setlocale(\"%s\") failed after being accepted", pcl_name[k]);
		for (v = 0; v < 0x10000L; v++) {
			wchar_t w = (wchar_t)v;
			char buf[MB_LEN_MAX];
			mbstate_t st;
			int dig, xd;

			memset(&st, 0, sizeof st);
			if (wcrtomb(buf, w, &st) == (size_t)-1)
				continue;
			dig = v < 0x100 && v != 0 && strchr(pcc_digit, (int)v) != NULL;
			xd = dig || (v < 0x100 && v != 0 && (strchr(pcc_xup, (int)v) != NULL || strchr(pcc_xlow, (int)v) != NULL));
			pc_check((iswdigit((wint_t)w) != 0) == dig, "locale %s: iswdigit(%#lx) is %d, expected %d", pcl_name[k], v,
			    iswdigit((wint_t)w) != 0, dig);
			pc_check((iswxdigit((wint_t)w) != 0) == xd, "locale %s: iswxdigit(%#lx) is %d, expected %d", pcl_name[k], v,
			    iswxdigit((wint_t)w) != 0, xd);
		}
	}
	(void)setlocale(LC_ALL, "C");
	pc_pass();
}
