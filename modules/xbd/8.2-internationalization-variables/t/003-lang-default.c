/*
 * LANG decides every category for which neither LC_ALL nor the category's
 * own variable is set to a non-null value; unset and null variables are
 * alike.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.8.2-internationalization-variables.003
 */
#include "pc.h"
#include "xloc.h"

int main(void)
{
	const char *per[6];
	int all[6] = { 1, 1, 1, 1, 1, 1 };
	int i;

	pc_begin("XBD.8.2-internationalization-variables.003");
	ld_need();
	xl_make();
	pc_test();
	xl_env(NULL, xl_loc, NULL);
	xl_select();
	xl_expect("only LANG=X set", all);
	for (i = 0; i < 6; i++)
		per[i] = "";
	xl_env("", xl_loc, per);
	xl_select();
	xl_expect("LANG=X, LC_ALL and every LC_* null", all);
	for (i = 0; i < 6; i++)
		per[i] = NULL;
	per[1] = "";
	per[4] = "";
	xl_env(NULL, xl_loc, per);
	xl_select();
	xl_expect("LANG=X, LC_CTYPE and LC_NUMERIC null", all);
	/* a null LANG selects nothing: the category variable alone decides its category */
	for (i = 0; i < 6; i++)
		per[i] = NULL;
	per[3] = xl_loc;
	xl_env(NULL, "", per);
	xl_select();
	pc_check(xl_obs(LC_MONETARY) == 1, "LANG null, LC_MONETARY=X: LC_MONETARY does not behave as locale X");
	ld_unset_all();
	(void)setlocale(LC_ALL, "C");
	pc_pass();
}
