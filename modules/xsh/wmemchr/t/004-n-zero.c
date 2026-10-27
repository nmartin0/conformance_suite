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
	r = wmemchr(w, L'a', 0);
	pc_check(r == NULL, "wmemchr() with n 0 did not return a null pointer");
	pc_pass();
}
