/*
 * wcsrtombs() converts up to and including the terminating null wide
 * character, sets *src to a null pointer and leaves the initial state.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcsrtombs.001
 */
#include "pc.h"
#include <string.h>
#include <wchar.h>

int main(void)
{
	static const wchar_t ws[] = { L'a', L'b', L'c', 0, L'z', L'z', 0 };
	mbstate_t st;
	const wchar_t *src = ws;
	char d[8];
	size_t r;
	int init;

	pc_begin("XSH.wcsrtombs.001");
	memset(&st, 0, sizeof st);
	memset(d, 'x', sizeof d);
	pc_test();
	r = wcsrtombs(d, &src, 8, &st);
	init = mbsinit(&st);
	pc_check(r == 3, "wcsrtombs() returned %zu, expected 3", r);
	pc_check(memcmp(d, "abc", 3) == 0, "wcsrtombs() stored the wrong bytes");
	pc_check(d[3] == '\0', "wcsrtombs() stored %d after the characters, expected the terminating null byte", d[3]);
	pc_check(src == NULL, "wcsrtombs() left *src non-null after reaching the null wide character");
	pc_check(init != 0, "wcsrtombs() left a state that is not initial after reaching the null wide character");
	pc_pass();
}
