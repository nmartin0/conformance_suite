/*
 * wcsstr() with an empty second string returns the first argument.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcsstr.002
 */
#include "pc.h"
#include <wchar.h>

static wchar_t abc[] = { L'a', L'b', L'c', 0 }, none[] = { 0 }, none2[] = { 0 };

int main(void)
{
	wchar_t *r1, *r2;

	pc_begin("XSH.wcsstr.002");
	pc_test();
	r1 = wcsstr(abc, none);
	r2 = wcsstr(none2, none);
	pc_check(r1 == abc, "(\"abc\", \"\") did not return its first argument");
	pc_check(r2 == none2, "(\"\", \"\") did not return its first argument");
	pc_pass();
}
