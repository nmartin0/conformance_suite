/*
 * wcscspn() leaves errno alone.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcscspn.003
 */
#include "pc.h"
#include <errno.h>
#include <wchar.h>

/* The result is kept so that the call cannot be dropped as unused. */
static volatile long sink;

static wchar_t s[] = { L'a', L'b', L'c', 0 }, set[] = { L'c', 0 };

int main(void)
{
	static const int before[2] = { 0, EDOM };
	int i, e;

	pc_begin("XSH.wcscspn.003");
	pc_test();
	for (i = 0; i < 2; i++) {
		errno = before[i];
		sink = (long)wcscspn(s, set);
		e = errno;
		pc_check(e == before[i], "wcscspn() changed errno from %s to %s", pc_errname(before[i]), pc_errname(e));
	}
	pc_pass();
}
