/*
 * wcstombs() modifies no more than n bytes, and stores no terminating null
 * byte when it returns n.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcstombs.002
 */
#include "pc.h"
#include <stdlib.h>
#include <string.h>

int main(void)
{
	char s[8];
	size_t r1, r2;

	pc_begin("XSH.wcstombs.002");
	pc_test();
	memset(s, 'x', sizeof s);
	r1 = wcstombs(s, L"abcd", 2);
	pc_check(r1 == 2, "wcstombs(s, L\"abcd\", 2) returned %zu, expected 2", r1);
	pc_check(s[0] == 'a' && s[1] == 'b', "wcstombs() stored the wrong bytes");
	pc_check(s[2] == 'x', "wcstombs() modified the byte after the n-th");
	memset(s, 'x', sizeof s);
	r2 = wcstombs(s, L"abc", 3);
	pc_check(r2 == 3, "wcstombs(s, L\"abc\", 3) returned %zu, expected 3", r2);
	pc_check(s[3] == 'x', "wcstombs() modified the byte after the n-th when the string had exactly n characters");
	pc_pass();
}
