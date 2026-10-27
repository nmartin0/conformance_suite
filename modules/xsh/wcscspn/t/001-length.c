/*
 * wcscspn() measures the initial segment free of the given codes.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcscspn.001
 */
#include "pc.h"
#include <wchar.h>

static wchar_t s[] = { L'a', L'b', L'c', L'd', L'e', 0 };
static wchar_t dc[] = { L'd', L'c', 0 }, ed[] = { L'e', L'd', 0 }, xyz[] = { L'x', L'y', L'z', 0 };
static wchar_t a[] = { L'a', 0 }, none[] = { 0 };

int main(void)
{
	size_t r[6];

	pc_begin("XSH.wcscspn.001");
	pc_test();
	r[0] = wcscspn(s, dc);
	r[1] = wcscspn(s, ed);
	r[2] = wcscspn(s, xyz);
	r[3] = wcscspn(s, a);
	r[4] = wcscspn(s, none);
	r[5] = wcscspn(none, a);
	pc_check(r[0] == 2, "(\"abcde\", \"dc\") returned %zu, expected 2", r[0]);
	pc_check(r[1] == 3, "(\"abcde\", \"ed\") returned %zu, expected 3", r[1]);
	pc_check(r[2] == 5, "(\"abcde\", \"xyz\") returned %zu, expected 5", r[2]);
	pc_check(r[3] == 0, "(\"abcde\", \"a\") returned %zu, expected 0", r[3]);
	pc_check(r[4] == 5, "(\"abcde\", \"\") returned %zu, expected 5", r[4]);
	pc_check(r[5] == 0, "(\"\", \"a\") returned %zu, expected 0", r[5]);
	pc_pass();
}
