/*
 * wcsspn() leaves errno alone.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcsspn.003
 */
#include "pc.h"
#include <errno.h>
#include <wchar.h>

static wchar_t s[] = { L'a', L'b', L'c', 0 }, set[] = { L'a', 0 };

int main(void)
{
	static const int before[2] = { 0, EDOM };
	int i, e;

	pc_begin("XSH.wcsspn.003");
	pc_test();
	for (i = 0; i < 2; i++) {
		errno = before[i];
		(void)wcsspn(s, set);
		e = errno;
		pc_check(e == before[i], "wcsspn() changed errno from %s to %s", pc_errname(before[i]), pc_errname(e));
	}
	pc_pass();
}
