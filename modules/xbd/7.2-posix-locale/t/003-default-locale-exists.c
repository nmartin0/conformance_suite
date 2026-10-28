/*
 * The implementation defines a default locale for when no environment
 * variable is set or all are empty: selecting the locale from the
 * environment succeeds in both cases and leaves a locale in which the
 * portable characters still work.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.7.2-posix-locale.003
 */
#include "pc.h"
#include "ldef.h"
#include <ctype.h>

static void check_usable(const char *how)
{
	pc_check(isalpha((unsigned char)pcc_lower[0]) != 0 && isdigit((unsigned char)pcc_digit[3]) != 0 &&
	    isspace((unsigned char)' ') != 0, "after %s the portable characters do not classify as letter, digit and space", how);
}

int main(void)
{
	int i;

	pc_begin("XBD.7.2-posix-locale.003");
	pc_test();
	ld_unset_all();
	pc_check(setlocale(LC_ALL, "") != NULL, "setlocale(LC_ALL, \"\") failed with no locale variable set");
	check_usable("setlocale with no variable set");
	pc_check(setlocale(LC_ALL, "C") != NULL, "setlocale(LC_ALL, \"C\") failed afterwards");
	for (i = 0; ld_vars[i] != NULL; i++)
		pc_check(setenv(ld_vars[i], "", 1) == 0, "setenv failed");
	pc_check(setlocale(LC_ALL, "") != NULL, "setlocale(LC_ALL, \"\") failed with every locale variable empty");
	check_usable("setlocale with every variable empty");
	ld_unset_all();
	(void)setlocale(LC_ALL, "C");
	pc_pass();
}
