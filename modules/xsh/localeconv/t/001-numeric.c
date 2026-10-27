/*
 * localeconv() in the POSIX locale: the numeric members.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.localeconv.001
 */
#include "lconv.h"

int main(void)
{
	pc_begin("XSH.localeconv.001");
	lc_init();
	if (setlocale(LC_ALL, "C") == NULL)
		pc_unresolved("setlocale(LC_ALL, \"C\") failed");
	pc_test();
	lcv_numeric(localeconv(), "POSIX locale");
	pc_pass();
}
