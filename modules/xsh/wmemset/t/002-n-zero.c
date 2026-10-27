/*
 * wmemset() with n 0 writes nothing.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wmemset.002
 */
#include "pc.h"
#include <wchar.h>

int main(void)
{
	wchar_t buf[4], *r;
	int i;

	pc_begin("XSH.wmemset.002");
	for (i = 0; i < 4; i++)
		buf[i] = L'X';
	pc_test();
	r = wmemset(buf, L'q', 0);
	pc_check(r == buf, "wmemset() did not return its first argument");
	for (i = 0; i < 4; i++)
		pc_check(buf[i] == L'X', "buf[%d] was changed", i);
	pc_pass();
}
