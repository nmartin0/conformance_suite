/*
 * wmemcmp() returns 0 for equal content, and for n 0.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wmemcmp.001
 */
#include "pc.h"
#include <wchar.h>

static wchar_t a[] = { L'a', 0, L'b' }, b[] = { L'a', 0, L'b' };
static wchar_t c[] = { L'a', L'b' }, d[] = { L'c', L'd' };

int main(void)
{
	int r1, r2;

	pc_begin("XSH.wmemcmp.001");
	pc_test();
	r1 = wmemcmp(a, b, 3);
	r2 = wmemcmp(c, d, 0);
	pc_check(r1 == 0, "equal arrays (with a null code) returned %d", r1);
	pc_check(r2 == 0, "n 0 on different arrays returned %d", r2);
	pc_pass();
}
