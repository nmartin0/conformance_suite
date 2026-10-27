/*
 * wcsrtombs() stops at a code that is no character with (size_t)-1 and
 * EILSEQ, having stored the characters before it and left *src just past
 * them. The POSIX locale has 256 characters, so among the codes 1 to 257 at
 * least one is invalid.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcsrtombs.005
 */
#include "pc.h"
#include <errno.h>
#include <string.h>
#include <wchar.h>

int main(void)
{
	mbstate_t st;
	wchar_t one[2] = { 0, 0 }, mid[4];
	const wchar_t *src;
	char d[16];
	size_t r;
	int v, e, found = -1;

	pc_begin("XSH.wcsrtombs.005");
	pc_test();
	for (v = 1; v <= 257 && found < 0; v++) {
		one[0] = (wchar_t)v;
		src = one;
		memset(&st, 0, sizeof st);
		if (wcsrtombs(NULL, &src, 0, &st) == (size_t)-1)
			found = v;
	}
	pc_check(found > 0, "wcsrtombs() accepted all codes 1 to 257, but the locale has 256 characters");
	mid[0] = L'a';
	mid[1] = (wchar_t)found;
	mid[2] = L'b';
	mid[3] = 0;
	src = mid;
	memset(&st, 0, sizeof st);
	memset(d, 'x', sizeof d);
	errno = EDOM;
	r = wcsrtombs(d, &src, sizeof d, &st);
	e = errno;
	pc_check(r == (size_t)-1, "wcsrtombs() of a string with an invalid code returned %zu, expected (size_t)-1", r);
	pc_check(e == EILSEQ, "wcsrtombs() set errno to %s, expected EILSEQ", pc_errname(e));
	pc_check(d[0] == 'a', "wcsrtombs() did not store the character before the invalid code");
	pc_check(src == mid + 1, "wcsrtombs() left *src at the wrong place; expected just past the last converted character");
	pc_pass();
}
