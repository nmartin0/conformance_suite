/*
 * With LC_ALL unset or null, a non-null category variable LC_x decides
 * its category alone and over LANG: the other categories follow LANG.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.8.2-internationalization-variables.002
 */
#include "pc.h"
#include "xloc.h"

int main(void)
{
	const char *per[6];
	int want[6], c, i, v;
	char what[120];

	pc_begin("XBD.8.2-internationalization-variables.002");
	ld_need();
	xl_make();
	pc_test();
	for (v = 0; v < 2; v++) {
		for (c = 0; c < 6; c++) {
			for (i = 0; i < 6; i++) {
				per[i] = NULL;
				want[i] = v;
			}
			per[c] = v ? "POSIX" : xl_loc;
			want[c] = !v;
			/* v = 0: LANG names POSIX, LC_x names X; v = 1: LANG names X, LC_x names POSIX */
			xl_env(NULL, v ? xl_loc : "POSIX", per);
			xl_select();
			(void)snprintf(what, sizeof what, "LC_ALL unset, LANG=%s, %s=%s", v ? "X" : "POSIX", xl_vars[c], v ? "POSIX" : "X");
			xl_expect(what, want);
			xl_env("", v ? xl_loc : "POSIX", per);
			xl_select();
			(void)snprintf(what, sizeof what, "LC_ALL null, LANG=%s, %s=%s", v ? "X" : "POSIX", xl_vars[c], v ? "POSIX" : "X");
			xl_expect(what, want);
		}
	}
	ld_unset_all();
	(void)setlocale(LC_ALL, "C");
	pc_pass();
}
