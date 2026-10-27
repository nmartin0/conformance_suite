/*
 * localeconv() in the POSIX locale: the monetary strings.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.localeconv.002
 */
#include "lconv.h"

int main(void)
{
	pc_begin("XSH.localeconv.002");
	lc_init();
	if (setlocale(LC_ALL, "C") == NULL)
		pc_unresolved("setlocale(LC_ALL, \"C\") failed");
	pc_test();
	lcv_mstr(localeconv(), "POSIX locale");
	pc_pass();
}
