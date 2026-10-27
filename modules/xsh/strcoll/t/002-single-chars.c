/*
 * The collation order of single characters in the POSIX locale.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strcoll.002
 */
#include "coll.h"

int main(void)
{
	pc_begin("XSH.strcoll.002");
	lc_init();
	if (setlocale(LC_ALL, "C") == NULL)
		pc_unresolved("setlocale(LC_ALL, \"C\") failed");
	pc_test();
	coll_obj = (locale_t)0;
	coll_check_singles("strcoll");
	pc_pass();
}
