/*
 * wcsncat() with n 0 leaves the destination string as it was.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcsncat.003
 */
#include "pc.h"
#include <wchar.h>

int main(void)
{
	wchar_t d[8], src[] = { L'c', L'd', 0 };
	int i;

	pc_begin("XSH.wcsncat.003");
	for (i = 0; i < 8; i++)
		d[i] = L'X';
	d[0] = L'a';
	d[1] = L'b';
	d[2] = 0;
	pc_test();
	/* control: with n 2 the two codes are appended (on a copy), so n 0 leaving d alone is the function's doing */
	{
		wchar_t c[8] = { L'a', L'b', 0 };

		(void)wcsncat(c, src, 2);
		pc_check(c[2] == L'c' && c[3] == L'd' && c[4] == 0, "wcsncat() with n 2 did not append the two codes");
	}
	(void)wcsncat(d, src, 0);
	pc_check(d[0] == L'a' && d[1] == L'b', "the string was changed");
	pc_check(d[2] == 0, "the null code at index 2 is gone");
	for (i = 3; i < 8; i++)
		pc_check(d[i] == L'X', "d[%d] after the string was changed", i);
	pc_pass();
}
