/*
 * The global locale at the start of the program is the POSIX locale.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.setlocale.001
 */
#include "lc.h"

int main(void)
{
	const struct lconv *lc;
	int i;

	pc_begin("XSH.setlocale.001");
	lc_init();
	pc_test();
	pc_check(setlocale(LC_ALL, NULL) != NULL, "setlocale(LC_ALL, NULL) returned a null pointer");
	for (i = 0; i < 6; i++)
		pc_check(setlocale(lc_cat[i], NULL) != NULL, "setlocale(%d, NULL) returned a null pointer", lc_cat[i]);
	lc_check_items((locale_t)0, "initial locale");
	lc_check_cat((locale_t)0, LC_ALL, "initial locale");
	lc = localeconv();
	pc_check(lc != NULL && strcmp(lc->decimal_point, ".") == 0, "localeconv() does not show the POSIX decimal point");
	pc_pass();
}
