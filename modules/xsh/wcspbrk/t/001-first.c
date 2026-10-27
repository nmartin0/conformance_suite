/*
 * wcspbrk() returns the first code of the string that is in the set.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcspbrk.001
 */
#include "pc.h"
#include <wchar.h>

static wchar_t s[] = { L'h', L'e', L'l', L'l', L'o', L' ', L'w', L'o', L'r', L'l', L'd', 0 };
static wchar_t ow[] = { L'o', L'w', 0 }, wo[] = { L'w', L'o', 0 }, h[] = { L'h', 0 };

int main(void)
{
	wchar_t *r[3];

	pc_begin("XSH.wcspbrk.001");
	pc_test();
	r[0] = wcspbrk(s, ow);
	r[1] = wcspbrk(s, wo);
	r[2] = wcspbrk(s, h);
	pc_check(r[0] == s + 4, "(\"hello world\", \"ow\") did not return a pointer to index 4");
	pc_check(r[1] == s + 4, "(\"hello world\", \"wo\") did not return a pointer to index 4");
	pc_check(r[2] == s, "(\"hello world\", \"h\") did not return its first argument");
	pc_pass();
}
