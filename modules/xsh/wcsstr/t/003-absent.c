/*
 * wcsstr() returns a null pointer when the substring is not found.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcsstr.003
 */
#include "pc.h"
#include <wchar.h>

static wchar_t s[] = { L'a', L'b', L'c', L'a', L'b', L'c', 0 }, bd[] = { L'b', L'd', 0 };
static wchar_t abc[] = { L'a', L'b', L'c', 0 }, abcd[] = { L'a', L'b', L'c', L'd', 0 };
static wchar_t split[] = { L'a', L'b', 0, L'c', L'd', 0 }, bc[] = { L'b', L'c', 0 };

int main(void)
{
	wchar_t *r[3];

	pc_begin("XSH.wcsstr.003");
	pc_test();
	/* control: a string that is present is found */
	pc_check(wcsstr(s, bc) == s + 1, "\"bc\" was not found in \"abcabc\"");
	r[0] = wcsstr(s, bd);
	r[1] = wcsstr(abc, abcd);
	r[2] = wcsstr(split, bc);
	pc_check(r[0] == NULL, "\"bd\" was found in \"abcabc\"");
	pc_check(r[1] == NULL, "\"abcd\" was found in \"abc\"");
	pc_check(r[2] == NULL, "a match was found across the null code");
	pc_pass();
}
