/*
 * wcstombs() converts a wide string and stores the terminating null byte
 * when there is room.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcstombs.001
 */
#include "pc.h"
#include <stdlib.h>
#include <string.h>

int main(void)
{
	char s[8];
	size_t r;

	pc_begin("XSH.wcstombs.001");
	memset(s, 'x', sizeof s);
	pc_test();
	r = wcstombs(s, L"abc", 8);
	pc_check(r == 3, "wcstombs() returned %zu, expected 3", r);
	pc_check(memcmp(s, "abc", 3) == 0, "wcstombs() stored the wrong bytes");
	pc_check(s[3] == '\0', "wcstombs() stored %d after the characters, expected a null byte", s[3]);
	pc_pass();
}
