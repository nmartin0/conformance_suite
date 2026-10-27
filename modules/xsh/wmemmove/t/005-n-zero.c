/*
 * wmemmove() with n 0 changes nothing.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wmemmove.005
 */
#include "pc.h"
#include <wchar.h>

int main(void)
{
	wchar_t buf[] = { L'a', L'b', L'c', L'd' }, *r;
	const wchar_t want[4] = { L'a', L'b', L'c', L'd' };
	int i;

	pc_begin("XSH.wmemmove.005");
	pc_test();
	r = wmemmove(buf + 1, buf, 0);
	pc_check(r == buf + 1, "wmemmove() did not return its first argument");
	for (i = 0; i < 4; i++)
		pc_check(buf[i] == want[i], "buf[%d] was changed", i);
	pc_pass();
}
