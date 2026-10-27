/*
 * wcslcpy() with size 0 writes nothing and returns the source length.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcslcat.003
 */
#include "pc.h"
#include <wchar.h>

int main(void)
{
	wchar_t d[8], src[] = { L'a', L'b', L'c', L'd', L'e', L'f', 0 };
	size_t r;
	int i;

	pc_begin("XSH.wcslcat.003");
	for (i = 0; i < 8; i++)
		d[i] = L'X';
	pc_test();
	r = wcslcpy(d, src, 0);
	pc_check(r == 6, "wcslcpy() with size 0 returned %zu, expected 6", r);
	for (i = 0; i < 8; i++)
		pc_check(d[i] == L'X', "d[%d] was changed", i);
	pc_pass();
}
