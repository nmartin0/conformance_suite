/*
 * A non-null LC_ALL decides every category over every LC_* variable and
 * over LANG: with LC_ALL naming the POSIX locale a category variable and
 * LANG naming another locale have no effect, and with LC_ALL naming that
 * locale, category variables and LANG naming the POSIX locale have none.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.8.2-internationalization-variables.001
 */
#include "pc.h"
#include "xloc.h"

int main(void)
{
	const char *per[6];
	int none[6] = { 0, 0, 0, 0, 0, 0 }, all[6] = { 1, 1, 1, 1, 1, 1 };
	int c, i;
	char what[100];

	pc_begin("XBD.8.2-internationalization-variables.001");
	ld_need();
	xl_make();
	pc_test();
	for (c = 0; c < 6; c++) {
		for (i = 0; i < 6; i++)
			per[i] = NULL;
		per[c] = xl_loc;
		xl_env("POSIX", xl_loc, per);
		xl_select();
		(void)snprintf(what, sizeof what, "LC_ALL=POSIX, %s and LANG naming X", xl_vars[c]);
		xl_expect(what, none);
		for (i = 0; i < 6; i++)
			per[i] = NULL;
		per[c] = "POSIX";
		xl_env(xl_loc, "POSIX", per);
		xl_select();
		(void)snprintf(what, sizeof what, "LC_ALL=X, %s and LANG naming POSIX", xl_vars[c]);
		xl_expect(what, all);
	}
	for (i = 0; i < 6; i++)
		per[i] = "POSIX";
	xl_env(xl_loc, "POSIX", per);
	xl_select();
	xl_expect("LC_ALL=X, every LC_* and LANG naming POSIX", all);
	ld_unset_all();
	(void)setlocale(LC_ALL, "C");
	pc_pass();
}
