/*
 * wmemmove() between distinct arrays copies n elements and returns the destination.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wmemmove.001
 */
#include "pc.h"
#include <wchar.h>

int main(void)
{
	wchar_t d[10], src[] = { L'a', L'b', L'c', L'd', L'e' }, *r;
	int i;

	pc_begin("XSH.wmemmove.001");
	for (i = 0; i < 10; i++)
		d[i] = L'X';
	pc_test();
	r = wmemmove(d, src, 5);
	pc_check(r == d, "wmemmove() did not return its first argument");
	for (i = 0; i < 5; i++)
		pc_check(d[i] == src[i], "element %d was not copied", i);
	for (i = 5; i < 10; i++)
		pc_check(d[i] == L'X', "d[%d] after the n elements was changed", i);
	pc_pass();
}
