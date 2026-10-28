/*
 * wmemchr() with n 0 finds nothing.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wmemchr.004
 */
#include "pc.h"
#include <wchar.h>

static wchar_t w[] = { L'a', L'b' };

int main(void)
{
	wchar_t *r;

	pc_begin("XSH.wmemchr.004");
	pc_test();
	/* control: with n 1 the code is found, so the null result for n 0 is the function's doing */
	pc_check(wmemchr(w, L'a', 1) == w, "wmemchr() with n 1 did not find the code at index 0");
	r = wmemchr(w, L'a', 0);
	pc_check(r == NULL, "wmemchr() with n 0 did not return a null pointer");
	pc_pass();
}
