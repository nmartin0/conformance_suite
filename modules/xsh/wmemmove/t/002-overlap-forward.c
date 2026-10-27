/*
 * wmemmove() with the destination after the source, overlapping.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wmemmove.002
 */
#include "pc.h"
#include <wchar.h>

int main(void)
{
	wchar_t buf[] = { L'a', 0, WCHAR_MAX, L'd', L'e', L'f' }, *r;
	const wchar_t want[6] = { L'a', 0, L'a', 0, WCHAR_MAX, L'd' };
	int i;

	pc_begin("XSH.wmemmove.002");
	pc_test();
	r = wmemmove(buf + 2, buf, 4);
	pc_check(r == buf + 2, "wmemmove() did not return its first argument");
	for (i = 0; i < 6; i++)
		pc_check(buf[i] == want[i], "buf[%d] has the wrong value after the move", i);
	pc_pass();
}
