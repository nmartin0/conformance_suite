/*
 * wcscasecmp() ignores case.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcscasecmp.001
 */
#include "pc.h"
#include <locale.h>
#include <wchar.h>

static wchar_t a1[] = L"abc", a2[] = L"ABC", b1[] = L"AbC", b2[] = L"aBc", c1[] = L"a1_", c2[] = L"A1_";
static wchar_t e1[1], e2[1];

int main(void)
{
	int r[4];

	pc_begin("XSH.wcscasecmp.001");
	pc_check(setlocale(LC_ALL, "C") != NULL, "setlocale(LC_ALL, \"C\") failed");
	pc_test();
	r[0] = wcscasecmp(a1, a2);
	r[1] = wcscasecmp(b1, b2);
	r[2] = wcscasecmp(c1, c2);
	r[3] = wcscasecmp(e1, e2);
	pc_check(r[0] == 0, "\"abc\" against \"ABC\" returned %d", r[0]);
	pc_check(r[1] == 0, "\"AbC\" against \"aBc\" returned %d", r[1]);
	pc_check(r[2] == 0, "\"a1_\" against \"A1_\" returned %d", r[2]);
	pc_check(r[3] == 0, "two empty strings returned %d", r[3]);
	pc_pass();
}
