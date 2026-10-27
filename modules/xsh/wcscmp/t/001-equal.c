/*
 * wcscmp() returns 0 for equal strings.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcscmp.001
 */
#include "pc.h"
#include <wchar.h>

static wchar_t a1[] = { L'a', L'b', L'c', 0 }, a2[] = { L'a', L'b', L'c', 0 };
static wchar_t e1[] = { 0 }, e2[] = { 0 };

int main(void)
{
	int r1, r2;

	pc_begin("XSH.wcscmp.001");
	pc_test();
	r1 = wcscmp(a1, a2);
	r2 = wcscmp(e1, e2);
	pc_check(r1 == 0, "wcscmp() of two equal \"abc\" arrays returned %d", r1);
	pc_check(r2 == 0, "wcscmp() of two empty strings returned %d", r2);
	pc_pass();
}
