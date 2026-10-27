/*
 * With a null dst wcsrtombs() counts the bytes needed whatever len is.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcsrtombs.003
 */
#include "pc.h"
#include <string.h>
#include <wchar.h>

int main(void)
{
	mbstate_t st;
	const wchar_t *src = L"abcd";
	size_t r1, r2;

	pc_begin("XSH.wcsrtombs.003");
	memset(&st, 0, sizeof st);
	pc_test();
	r1 = wcsrtombs(NULL, &src, 1, &st);
	src = L"abcd";
	r2 = wcsrtombs(NULL, &src, 100, &st);
	pc_check(r1 == 4, "wcsrtombs(NULL, &src, 1, &st) returned %zu, expected 4", r1);
	pc_check(r2 == 4, "wcsrtombs(NULL, &src, 100, &st) returned %zu, expected 4", r2);
	pc_pass();
}
