/*
 * wcsnrtombs() converts at most nwc wide characters and leaves *src just
 * past the last one; reaching the null wide character gives a null *src.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcsrtombs.007
 */
#include "pc.h"
#include <string.h>
#include <wchar.h>

int main(void)
{
	static const wchar_t ws[] = { L'a', L'b', L'c', L'd', L'e', L'f', 0 };
	mbstate_t st;
	const wchar_t *src;
	char d[16];
	size_t r1, r2, r3;

	pc_begin("XSH.wcsrtombs.007");
	pc_test();
	memset(&st, 0, sizeof st);
	src = ws;
	memset(d, 'x', sizeof d);
	r1 = wcsnrtombs(d, &src, 3, 16, &st);
	pc_check(r1 == 3, "wcsnrtombs() with nwc 3 returned %zu, expected 3", r1);
	pc_check(memcmp(d, "abc", 3) == 0, "wcsnrtombs() stored the wrong bytes");
	pc_check(d[3] == 'x', "wcsnrtombs() stored a byte beyond the nwc characters");
	pc_check(src == ws + 3, "wcsnrtombs() left *src at the wrong place after nwc characters");
	memset(&st, 0, sizeof st);
	src = ws;
	memset(d, 'x', sizeof d);
	r2 = wcsnrtombs(d, &src, 7, 16, &st);
	pc_check(r2 == 6, "wcsnrtombs() with nwc 7 returned %zu, expected 6", r2);
	pc_check(d[6] == '\0', "wcsnrtombs() did not store the terminating null byte");
	pc_check(src == NULL, "wcsnrtombs() left *src non-null after reaching the null wide character");
	memset(&st, 0, sizeof st);
	src = ws;
	r3 = wcsnrtombs(NULL, &src, 3, 1, &st);
	pc_check(r3 == 3, "wcsnrtombs() with a null dst and nwc 3 returned %zu, expected 3", r3);
	pc_pass();
}
