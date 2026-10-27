/*
 * wcwidth() leaves errno alone.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcwidth.005
 */
#include "pc.h"
#include <errno.h>
#include <locale.h>
#include <wchar.h>

int main(void)
{
	static const int before[2] = { 0, EDOM };
	static const wchar_t wc[3] = { L'a', L'\0', L'\t' };
	int i, k, e;

	pc_begin("XSH.wcwidth.005");
	pc_check(setlocale(LC_ALL, "C") != NULL, "setlocale(LC_ALL, \"C\") failed");
	pc_test();
	for (i = 0; i < 2; i++)
		for (k = 0; k < 3; k++) {
			errno = before[i];
			(void)wcwidth(wc[k]);
			e = errno;
			pc_check(e == before[i], "wcwidth() of character %#x changed errno from %s to %s",
			    (unsigned)wc[k], pc_errname(before[i]), pc_errname(e));
		}
	pc_pass();
}
