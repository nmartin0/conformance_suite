/*
 * wcstombs() stops at the first null wide character.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcstombs.003
 */
#include "pc.h"
#include <stdlib.h>
#include <string.h>

int main(void)
{
	static const wchar_t ws[] = { L'a', L'b', 0, L'c', L'd', 0 };
	char s[8];
	size_t r;

	pc_begin("XSH.wcstombs.003");
	memset(s, 'x', sizeof s);
	pc_test();
	r = wcstombs(s, ws, 8);
	pc_check(r == 2, "wcstombs() returned %zu, expected 2", r);
	pc_check(s[3] == 'x' && s[4] == 'x', "wcstombs() stored bytes for characters after the null wide character");
	pc_pass();
}
