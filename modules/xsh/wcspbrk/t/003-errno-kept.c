/*
 * wcspbrk() leaves errno alone.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcspbrk.003
 */
#include "pc.h"
#include <errno.h>
#include <wchar.h>

static wchar_t s[] = { L'a', L'b', L'c', 0 }, hit[] = { L'c', 0 }, miss[] = { L'z', 0 };

int main(void)
{
	static const int before[2] = { 0, EDOM };
	int i, e;

	pc_begin("XSH.wcspbrk.003");
	pc_test();
	for (i = 0; i < 2; i++) {
		errno = before[i];
		(void)wcspbrk(s, hit);
		e = errno;
		pc_check(e == before[i], "wcspbrk() (found) changed errno from %s to %s", pc_errname(before[i]), pc_errname(e));
		errno = before[i];
		(void)wcspbrk(s, miss);
		e = errno;
		pc_check(e == before[i], "wcspbrk() (not found) changed errno from %s to %s", pc_errname(before[i]), pc_errname(e));
	}
	pc_pass();
}
