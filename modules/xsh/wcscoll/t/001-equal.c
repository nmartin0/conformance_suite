/*
 * wcscoll() returns 0 for equal strings.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcscoll.001
 */
#include "pc.h"
#include <locale.h>
#include <wchar.h>

static wchar_t a1[] = { L'a', L'b', L'c', 0 }, a2[] = { L'a', L'b', L'c', 0 };
static wchar_t e1[] = { 0 }, e2[] = { 0 };

int main(void)
{
	int r1, r2;

	pc_begin("XSH.wcscoll.001");
	pc_check(setlocale(LC_ALL, "C") != NULL, "setlocale(LC_ALL, \"C\") failed");
	pc_test();
	r1 = wcscoll(a1, a2);
	r2 = wcscoll(e1, e2);
	pc_check(r1 == 0, "wcscoll() of two equal \"abc\" arrays returned %d", r1);
	pc_check(r2 == 0, "wcscoll() of two empty strings returned %d", r2);
	pc_pass();
}
