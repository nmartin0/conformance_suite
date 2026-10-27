/*
 * wcspbrk() returns a null pointer when no code of the set occurs.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcspbrk.002
 */
#include "pc.h"
#include <wchar.h>

static wchar_t abc[] = { L'a', L'b', L'c', 0 }, xyz[] = { L'x', L'y', L'z', 0 };
static wchar_t none[] = { 0 }, a[] = { L'a', 0 };

int main(void)
{
	wchar_t *r[3];

	pc_begin("XSH.wcspbrk.002");
	pc_test();
	r[0] = wcspbrk(abc, xyz);
	r[1] = wcspbrk(abc, none);
	r[2] = wcspbrk(none, a);
	pc_check(r[0] == NULL, "(\"abc\", \"xyz\") did not return a null pointer");
	pc_check(r[1] == NULL, "(\"abc\", \"\") did not return a null pointer");
	pc_check(r[2] == NULL, "(\"\", \"a\") did not return a null pointer");
	pc_pass();
}
