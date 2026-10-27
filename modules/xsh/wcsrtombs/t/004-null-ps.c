/*
 * wcsrtombs() with a null ps uses its own state and converts in the POSIX
 * locale.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcsrtombs.004
 */
#include "pc.h"
#include <string.h>
#include <wchar.h>

int main(void)
{
	const wchar_t *src = L"abc";
	char d[8];
	size_t r;

	pc_begin("XSH.wcsrtombs.004");
	pc_test();
	r = wcsrtombs(d, &src, 8, NULL);
	pc_check(r == 3, "wcsrtombs() with a null ps returned %zu, expected 3", r);
	pc_check(memcmp(d, "abc", 4) == 0, "wcsrtombs() with a null ps stored the wrong bytes");
	pc_pass();
}
