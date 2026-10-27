/*
 * wcstok() leaves errno alone.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcstok.008
 */
#include "pc.h"
#include <errno.h>
#include <wchar.h>

static wchar_t comma[] = L",";

int main(void)
{
	static const int before[2] = { 0, EDOM };
	int i, k, e;

	pc_begin("XSH.wcstok.008");
	pc_test();
	for (i = 0; i < 2; i++) {
		wchar_t a[8] = { L'x', L',', L'y', 0 }, *st = NULL;

		for (k = 0; k < 3; k++) {
			errno = before[i];
			(void)wcstok(k == 0 ? a : NULL, comma, &st);
			e = errno;
			pc_check(e == before[i], "wcstok() call %d changed errno from %s to %s",
			    k + 1, pc_errname(before[i]), pc_errname(e));
		}
	}
	pc_pass();
}
