/*
 * wcrtomb() with a null s acts as a conversion of the null wide character
 * into an internal buffer, whatever wc is: it returns 1 from the initial
 * state and leaves the initial state.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcrtomb.004
 */
#include "pc.h"
#include <string.h>
#include <wchar.h>

int main(void)
{
	mbstate_t st;
	size_t r;
	int init;

	pc_begin("XSH.wcrtomb.004");
	memset(&st, 0, sizeof st);
	pc_test();
	r = wcrtomb(NULL, L'A', &st);
	init = mbsinit(&st);
	pc_check(r == 1, "wcrtomb(NULL, L'A', &st) returned %zu, expected 1", r);
	pc_check(init != 0, "wcrtomb() with a null s left a state that is not initial");
	pc_pass();
}
