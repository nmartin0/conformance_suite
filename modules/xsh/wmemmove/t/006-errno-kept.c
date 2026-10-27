/*
 * wmemmove() leaves errno alone.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wmemmove.006
 */
#include "pc.h"
#include <errno.h>
#include <wchar.h>

int main(void)
{
	static const int before[2] = { 0, EDOM };
	wchar_t buf[6] = { L'a', L'b', L'c', L'd', L'e', L'f' };
	int i, e;

	pc_begin("XSH.wmemmove.006");
	pc_test();
	for (i = 0; i < 2; i++) {
		errno = before[i];
		(void)wmemmove(buf + 1, buf, 4);
		e = errno;
		pc_check(e == before[i], "wmemmove() changed errno from %s to %s", pc_errname(before[i]), pc_errname(e));
	}
	pc_pass();
}
