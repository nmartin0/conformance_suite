/*
 * wmemset() stores the value in the first n elements and returns the pointer.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wmemset.001
 */
#include "pc.h"
#include <wchar.h>

int main(void)
{
	wchar_t buf[10], *r;
	int i;

	pc_begin("XSH.wmemset.001");
	for (i = 0; i < 10; i++)
		buf[i] = L'X';
	pc_test();
	r = wmemset(buf, L'q', 4);
	pc_check(r == buf, "wmemset() did not return its first argument");
	for (i = 0; i < 4; i++)
		pc_check(buf[i] == L'q', "buf[%d] was not set", i);
	for (i = 4; i < 10; i++)
		pc_check(buf[i] == L'X', "buf[%d] after the n elements was changed", i);
	pc_pass();
}
