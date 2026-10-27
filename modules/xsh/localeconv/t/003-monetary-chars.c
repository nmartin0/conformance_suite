/*
 * localeconv() in the POSIX locale: the char members are CHAR_MAX.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.localeconv.003
 */
#include "lconv.h"

int main(void)
{
	pc_begin("XSH.localeconv.003");
	lc_init();
	if (setlocale(LC_ALL, "C") == NULL)
		pc_unresolved("setlocale(LC_ALL, \"C\") failed");
	pc_test();
	lcv_mchar(localeconv(), "POSIX locale");
	pc_pass();
}
