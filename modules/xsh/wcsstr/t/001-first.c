/*
 * wcsstr() returns the first occurrence of the substring.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcsstr.001
 */
#include "pc.h"
#include <wchar.h>

static wchar_t s[] = { L'a', L'b', L'c', L'a', L'b', L'c', L'd', 0 };
static wchar_t bc[] = { L'b', L'c', 0 }, cd[] = { L'c', L'd', 0 }, d[] = { L'd', 0 };
static wchar_t aaab[] = { L'a', L'a', L'a', L'b', 0 }, aab[] = { L'a', L'a', L'b', 0 };

int main(void)
{
	wchar_t *r[5];

	pc_begin("XSH.wcsstr.001");
	pc_test();
	r[0] = wcsstr(s, bc);
	r[1] = wcsstr(s, cd);
	r[2] = wcsstr(s, s);
	r[3] = wcsstr(s, d);
	r[4] = wcsstr(aaab, aab);
	pc_check(r[0] == s + 1, "\"bc\" in \"abcabcd\" is at index 1");
	pc_check(r[1] == s + 5, "\"cd\" in \"abcabcd\" is at index 5");
	pc_check(r[2] == s, "the whole string is at index 0");
	pc_check(r[3] == s + 6, "\"d\" in \"abcabcd\" is at index 6");
	pc_check(r[4] == aaab + 1, "\"aab\" in \"aaab\" is at index 1");
	pc_pass();
}
