/*
 * wctob() leaves errno alone on valid input.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wctob.004
 */
#include "pc.h"
#include <errno.h>
#include <stdio.h>
#include <wchar.h>

int main(void)
{
	static const int before[2] = { 0, EDOM };
	wint_t w[3];
	volatile int sink;
	int i, j, e;

	pc_begin("XSH.wctob.004");
	w[0] = L'A';
	w[1] = 0;
	w[2] = btowc(0xE1);
	pc_check(w[2] != WEOF, "btowc(0xE1) returned WEOF");
	pc_test();
	for (i = 0; i < 2; i++)
		for (j = 0; j < 3; j++) {
			errno = before[i];
			sink = wctob(w[j]);
			e = errno;
			pc_check(e == before[i], "wctob() changed errno from %s to %s", pc_errname(before[i]), pc_errname(e));
		}
	(void)sink;
	pc_pass();
}
