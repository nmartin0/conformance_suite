/*
 * setlocale(LC_ALL, "") with a variable naming a missing locale changes nothing.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.setlocale.008
 */
#include "lc.h"

static char all[512], names[6][512];

static void snap(void)
{
	int i;

	lc_q(LC_ALL, all, sizeof all);
	for (i = 0; i < 6; i++)
		lc_q(lc_cat[i], names[i], sizeof names[i]);
}

static void same(int have, const char *w)
{
	char b[512];
	int i;

	lc_q(LC_ALL, b, sizeof b);
	pc_check(strcmp(all, b) == 0, "%s: the name of LC_ALL changed from \"%s\" to \"%s\"", w, all, b);
	for (i = 0; i < 6; i++) {
		lc_q(lc_cat[i], b, sizeof b);
		pc_check(strcmp(names[i], b) == 0, "%s: the name of category %d changed from \"%s\" to \"%s\"", w, lc_cat[i], names[i], b);
	}
	if (have)
		pc_check(strcmp(nl_langinfo(CODESET), "UTF-8") == 0, "%s: the code set became \"%s\"", w, nl_langinfo(CODESET));
	else
		lc_check_cat((locale_t)0, LC_ALL, w);
}

static void start(int have)
{
	if (setlocale(LC_ALL, "C") == NULL || (have && setlocale(LC_CTYPE, lc_loc) == NULL))
		pc_unresolved("could not set the starting locale");
	snap();
}

int main(void)
{
	static const char *const v[6] = { "LC_COLLATE", "LC_CTYPE", "LC_MESSAGES", "LC_MONETARY", "LC_NUMERIC", "LC_TIME" };
	int have, i;

	pc_begin("XSH.setlocale.008");
	lc_init();
	have = lc_find();
	lc_nosuch();
	pc_test();

	/* One LC_* variable names a missing locale; LC_CTYPE (set to C) would change the global locale if it were applied. */
	start(have);
	lc_env_clear();
	lc_env_set("LANG", "C");
	lc_env_set("LC_CTYPE", "C");
	lc_env_set("LC_TIME", LC_NOSUCH);
	pc_check(setlocale(LC_ALL, "") == NULL, "setlocale(LC_ALL, \"\") with LC_TIME naming a missing locale did not return a null pointer");
	same(have, "LC_TIME missing");

	/* LC_ALL names a missing locale, the rest are good. */
	start(have);
	lc_env_clear();
	lc_env_set("LANG", "C");
	for (i = 0; i < 6; i++)
		lc_env_set(v[i], "C");
	lc_env_set("LC_ALL", LC_NOSUCH);
	pc_check(setlocale(LC_ALL, "") == NULL, "setlocale(LC_ALL, \"\") with LC_ALL naming a missing locale did not return a null pointer");
	same(have, "LC_ALL missing");
	pc_pass();
}
