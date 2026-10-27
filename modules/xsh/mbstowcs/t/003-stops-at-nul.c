/*
 * mbstowcs() stops at the first null byte and converts nothing after it.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mbstowcs.003
 */
#include "pc.h"
#include <stdlib.h>

int main(void)
{
	static const char s[] = { 'a', 'b', '\0', 'c', 'd', '\0' };
	wchar_t w[8];
	size_t r, i;

	pc_begin("XSH.mbstowcs.003");
	for (i = 0; i < 8; i++)
		w[i] = 77;
	pc_test();
	r = mbstowcs(w, s, 8);
	pc_check(r == 2, "mbstowcs() returned %zu, expected 2", r);
	pc_check(w[3] == 77 && w[4] == 77, "mbstowcs() stored codes for characters after the null byte");
	pc_pass();
}
