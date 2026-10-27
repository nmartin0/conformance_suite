/*
 * MB_CUR_MAX is a positive size_t, 1 in the POSIX locale.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.stdlib.h.003
 */
#include <locale.h>
#include <stdlib.h>
#include "pc.h"

_Static_assert(_Generic(MB_CUR_MAX, size_t: 1, default: 0), "MB_CUR_MAX does not have type size_t");

int main(void)
{
	pc_begin("XBD.stdlib.h.003");
	pc_test();
	pc_check(MB_CUR_MAX > 0, "MB_CUR_MAX is not positive");
	pc_check(MB_CUR_MAX == 1, "MB_CUR_MAX is %zu in the default locale, expected 1", (size_t)MB_CUR_MAX);
	if (setlocale(LC_ALL, "C") == NULL)
		pc_unresolved("setlocale(LC_ALL, \"C\") failed");
	pc_check(MB_CUR_MAX == 1, "MB_CUR_MAX is %zu in the C locale, expected 1", (size_t)MB_CUR_MAX);
	if (setlocale(LC_ALL, "POSIX") != NULL)
		pc_check(MB_CUR_MAX == 1, "MB_CUR_MAX is %zu in the POSIX locale, expected 1", (size_t)MB_CUR_MAX);
	pc_pass();
}
