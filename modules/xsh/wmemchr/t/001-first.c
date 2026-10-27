/*
 * wmemchr() finds the first occurrence among the first n elements.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wmemchr.001
 */
#include "pc.h"
#include <wchar.h>

static wchar_t w[] = { L'a', L'b', L'c', L'a', L'b' };

int main(void)
{
	wchar_t *r[4];

	pc_begin("XSH.wmemchr.001");
	pc_test();
	r[0] = wmemchr(w, L'b', 5);
	r[1] = wmemchr(w, L'a', 5);
	r[2] = wmemchr(w, L'c', 2);
	r[3] = wmemchr(w, L'c', 3);
	pc_check(r[0] == w + 1, "the first 'b' is at index 1");
	pc_check(r[1] == w, "the first 'a' is at index 0");
	pc_check(r[2] == NULL, "'c' (at index 2) was found with n 2");
	pc_check(r[3] == w + 2, "'c' was not found at index 2 with n 3");
	pc_pass();
}
