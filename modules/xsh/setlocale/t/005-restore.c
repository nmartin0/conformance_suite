/*
 * The strings that queries return restore what was queried.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.setlocale.005
 */
#include "lc.h"

int main(void)
{
	char qa[512], ct[512], tm[512];

	pc_begin("XSH.setlocale.005");
	lc_init();
	lc_need();
	if (setlocale(LC_ALL, "C") == NULL || setlocale(LC_CTYPE, lc_loc) == NULL || setlocale(LC_TIME, lc_loc) == NULL)
		pc_unresolved("could not build a mixed global locale");
	lc_q(LC_ALL, qa, sizeof qa);
	lc_q(LC_CTYPE, ct, sizeof ct);
	lc_q(LC_TIME, tm, sizeof tm);
	pc_test();
	if (setlocale(LC_ALL, "C") == NULL)
		pc_unresolved("setlocale(LC_ALL, \"C\") failed");
	lc_check_cat((locale_t)0, LC_ALL, "after \"C\"");
	pc_check(setlocale(LC_ALL, qa) != NULL, "setlocale(LC_ALL, <the string a query of LC_ALL returned>) failed");
	pc_check(strcmp(nl_langinfo(CODESET), "UTF-8") == 0, "after restoring LC_ALL the code set is \"%s\"", nl_langinfo(CODESET));
	lc_check_cat((locale_t)0, LC_NUMERIC, "LC_NUMERIC after restoring LC_ALL");
	lc_check_cat((locale_t)0, LC_MESSAGES, "LC_MESSAGES after restoring LC_ALL");
	lc_check_cat((locale_t)0, LC_COLLATE, "LC_COLLATE after restoring LC_ALL");
	lc_check_cat((locale_t)0, LC_MONETARY, "LC_MONETARY after restoring LC_ALL");

	if (setlocale(LC_ALL, "C") == NULL)
		pc_unresolved("setlocale(LC_ALL, \"C\") failed");
	pc_check(setlocale(LC_CTYPE, ct) != NULL, "setlocale(LC_CTYPE, <the string a query of LC_CTYPE returned>) failed");
	pc_check(strcmp(nl_langinfo(CODESET), "UTF-8") == 0, "after restoring LC_CTYPE the code set is \"%s\"", nl_langinfo(CODESET));
	lc_check_cat((locale_t)0, LC_TIME, "LC_TIME after restoring LC_CTYPE");

	if (setlocale(LC_ALL, "C") == NULL)
		pc_unresolved("setlocale(LC_ALL, \"C\") failed");
	pc_check(setlocale(LC_TIME, tm) != NULL, "setlocale(LC_TIME, <the string a query of LC_TIME returned>) failed");
	lc_check_cat((locale_t)0, LC_CTYPE, "LC_CTYPE after restoring LC_TIME");
	pc_pass();
}
