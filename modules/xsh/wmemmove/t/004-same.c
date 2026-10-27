/*
 * wmemmove() with the same source and destination changes nothing.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wmemmove.004
 */
#include "pc.h"
#include <wchar.h>

int main(void)
{
	wchar_t buf[] = { L'a', L'b', 0, L'd' }, *r;
	const wchar_t want[4] = { L'a', L'b', 0, L'd' };
	int i;

	pc_begin("XSH.wmemmove.004");
	pc_test();
	r = wmemmove(buf, buf, 4);
	pc_check(r == buf, "wmemmove() did not return its first argument");
	for (i = 0; i < 4; i++)
		pc_check(buf[i] == want[i], "buf[%d] was changed", i);
	pc_pass();
}
