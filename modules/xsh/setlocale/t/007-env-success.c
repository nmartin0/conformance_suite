/*
 * setlocale(LC_ALL, "") with environment variables naming the POSIX locale.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.setlocale.007
 */
#include "lc.h"

static void start(int have)
{
	if (setlocale(LC_ALL, have ? lc_loc : "C") == NULL)
		pc_unresolved("could not set the starting locale");
}

int main(void)
{
	static const char *const v[6] = { "LC_COLLATE", "LC_CTYPE", "LC_MESSAGES", "LC_MONETARY", "LC_NUMERIC", "LC_TIME" };
	int have, i;

	pc_begin("XSH.setlocale.007");
	lc_init();
	have = lc_find();
	lc_nosuch();
	pc_test();

	/* LC_ALL takes precedence over the LC_* variables and LANG. */
	start(have);
	lc_env_clear();
	for (i = 0; i < 6; i++)
		lc_env_set(v[i], LC_NOSUCH);
	lc_env_set("LANG", LC_NOSUCH);
	lc_env_set("LC_ALL", "POSIX");
	pc_check(setlocale(LC_ALL, "") != NULL, "setlocale(LC_ALL, \"\") failed with LC_ALL=POSIX");
	lc_check_cat((locale_t)0, LC_ALL, "LC_ALL=POSIX");

	/* The LC_* variables take precedence over LANG. */
	start(have);
	lc_env_clear();
	for (i = 0; i < 6; i++)
		lc_env_set(v[i], "C");
	lc_env_set("LANG", LC_NOSUCH);
	pc_check(setlocale(LC_ALL, "") != NULL, "setlocale(LC_ALL, \"\") failed with every LC_* set to C");
	lc_check_cat((locale_t)0, LC_ALL, "LC_* = C");

	/* LANG alone. */
	start(have);
	lc_env_clear();
	lc_env_set("LANG", "POSIX");
	pc_check(setlocale(LC_ALL, "") != NULL, "setlocale(LC_ALL, \"\") failed with LANG=POSIX");
	lc_check_cat((locale_t)0, LC_ALL, "LANG=POSIX");
	pc_pass();
}
