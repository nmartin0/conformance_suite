/*
 * wcsspn() measures the initial segment made only of the given codes.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcsspn.001
 */
#include "pc.h"
#include <wchar.h>

static wchar_t s[] = { L'a', L'b', L'c', L'a', L'b', L'c', L'd', 0 };
static wchar_t cba[] = { L'c', L'b', L'a', 0 }, z[] = { L'z', 0 }, none[] = { 0 };
static wchar_t all[] = { L'a', L'b', L'a', 0 }, ab[] = { L'a', L'b', 0 };

int main(void)
{
	size_t r[5];

	pc_begin("XSH.wcsspn.001");
	pc_test();
	r[0] = wcsspn(s, cba);
	r[1] = wcsspn(s, z);
	r[2] = wcsspn(all, ab);
	r[3] = wcsspn(s, none);
	r[4] = wcsspn(none, ab);
	pc_check(r[0] == 6, "(\"abcabcd\", \"cba\") returned %zu, expected 6", r[0]);
	pc_check(r[1] == 0, "(\"abcabcd\", \"z\") returned %zu, expected 0", r[1]);
	pc_check(r[2] == 3, "(\"aba\", \"ab\") returned %zu, expected 3", r[2]);
	pc_check(r[3] == 0, "(\"abcabcd\", \"\") returned %zu, expected 0", r[3]);
	pc_check(r[4] == 0, "(\"\", \"ab\") returned %zu, expected 0", r[4]);
	pc_pass();
}
