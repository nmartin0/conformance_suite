/*
 * LC_ALL naming the C locale gives every category the POSIX behaviour,
 * whatever LANG and the category variables name.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.8.2-internationalization-variables.004
 */
#include "pc.h"
#include "xloc.h"

int main(void)
{
	const char *per[6];
	int none[6] = { 0, 0, 0, 0, 0, 0 };
	int i;

	pc_begin("XBD.8.2-internationalization-variables.004");
	ld_need();
	xl_make();
	pc_test();
	for (i = 0; i < 6; i++)
		per[i] = xl_loc;
	xl_env("C", xl_loc, per);
	xl_select();
	xl_expect("LC_ALL=C, LANG and every LC_* naming X", none);
	xl_env("POSIX", xl_loc, per);
	xl_select();
	xl_expect("LC_ALL=POSIX, LANG and every LC_* naming X", none);
	ld_unset_all();
	(void)setlocale(LC_ALL, "C");
	pc_pass();
}
