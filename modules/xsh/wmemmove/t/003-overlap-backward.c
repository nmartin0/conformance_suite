/*
 * wmemmove() with the destination before the source, overlapping.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wmemmove.003
 */
#include "pc.h"
#include <wchar.h>

int main(void)
{
	wchar_t buf[] = { L'a', L'b', L'c', L'd', L'e', L'f' }, *r;
	const wchar_t want[6] = { L'c', L'd', L'e', L'f', L'e', L'f' };
	int i;

	pc_begin("XSH.wmemmove.003");
	pc_test();
	r = wmemmove(buf, buf + 2, 4);
	pc_check(r == buf, "wmemmove() did not return its first argument");
	for (i = 0; i < 6; i++)
		pc_check(buf[i] == want[i], "buf[%d] has the wrong value after the move", i);
	pc_pass();
}
