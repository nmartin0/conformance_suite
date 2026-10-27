/*
 * wmemchr() treats the null code as ordinary data.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wmemchr.002
 */
#include "pc.h"
#include <wchar.h>

static wchar_t w[] = { L'a', 0, L'b', 0 };

int main(void)
{
	wchar_t *r[3];

	pc_begin("XSH.wmemchr.002");
	pc_test();
	r[0] = wmemchr(w, 0, 4);
	r[1] = wmemchr(w, L'b', 4);
	r[2] = wmemchr(w, L'z', 4);
	pc_check(r[0] == w + 1, "the null code was not found at index 1");
	pc_check(r[1] == w + 2, "'b' after the null code was not found at index 2");
	pc_check(r[2] == NULL, "'z' was reported present");
	pc_pass();
}
