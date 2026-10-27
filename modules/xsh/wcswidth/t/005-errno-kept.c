/*
 * wcswidth() leaves errno alone.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcswidth.005
 */
#include "pc.h"
#include <errno.h>
#include <locale.h>
#include <wchar.h>

int main(void)
{
	static const int before[2] = { 0, EDOM };
	int i, e;

	pc_begin("XSH.wcswidth.005");
	pc_check(setlocale(LC_ALL, "C") != NULL, "setlocale(LC_ALL, \"C\") failed");
	pc_test();
	for (i = 0; i < 2; i++) {
		errno = before[i];
		(void)wcswidth(L"abc", 3);
		e = errno;
		pc_check(e == before[i], "wcswidth() (printable) changed errno from %s to %s", pc_errname(before[i]), pc_errname(e));
		errno = before[i];
		(void)wcswidth(L"", 5);
		e = errno;
		pc_check(e == before[i], "wcswidth() (empty) changed errno from %s to %s", pc_errname(before[i]), pc_errname(e));
		errno = before[i];
		(void)wcswidth(L"a\tb", 3);
		e = errno;
		pc_check(e == before[i], "wcswidth() (non-printable) changed errno from %s to %s", pc_errname(before[i]), pc_errname(e));
	}
	pc_pass();
}
