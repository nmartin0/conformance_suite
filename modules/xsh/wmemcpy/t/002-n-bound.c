/*
 * wmemcpy() copies only n elements.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wmemcpy.002
 */
#include "pc.h"
#include <wchar.h>

int main(void)
{
	wchar_t d[8], src[] = { L'a', L'b', L'c', L'd' };
	int i;

	pc_begin("XSH.wmemcpy.002");
	for (i = 0; i < 8; i++)
		d[i] = L'X';
	pc_test();
	(void)wmemcpy(d, src, 2);
	pc_check(d[0] == L'a' && d[1] == L'b', "two elements were not copied");
	for (i = 2; i < 8; i++)
		pc_check(d[i] == L'X', "d[%d] after the n elements was changed", i);
	pc_pass();
}
