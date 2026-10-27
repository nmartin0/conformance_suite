/*
 * The numeric members come from LC_NUMERIC and the monetary ones from LC_MONETARY.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.localeconv.006
 */
#include "lconv.h"

int main(void)
{
	pc_begin("XSH.localeconv.006");
	lc_init();
	lc_need();
	pc_test();
	if (setlocale(LC_ALL, "C") == NULL || setlocale(LC_NUMERIC, lc_loc) == NULL)
		pc_unresolved("could not set LC_NUMERIC to the second locale");
	lcv_mstr(localeconv(), "only LC_NUMERIC changed");
	lcv_mchar(localeconv(), "only LC_NUMERIC changed");
	if (setlocale(LC_ALL, "C") == NULL || setlocale(LC_MONETARY, lc_loc) == NULL)
		pc_unresolved("could not set LC_MONETARY to the second locale");
	lcv_numeric(localeconv(), "only LC_MONETARY changed");
	(void)setlocale(LC_ALL, "C");
	pc_pass();
}
