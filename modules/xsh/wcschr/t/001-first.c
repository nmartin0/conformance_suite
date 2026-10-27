/*
 * wcschr() returns the first occurrence of the code.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcschr.001
 */
#include "pc.h"
#include <wchar.h>

static wchar_t ws[] = { L'a', L'b', L'c', L'a', L'b', L'c', 0 };

int main(void)
{
	wchar_t *a, *b, *c;

	pc_begin("XSH.wcschr.001");
	pc_test();
	a = wcschr(ws, L'a');
	b = wcschr(ws, L'b');
	c = wcschr(ws, L'c');
	pc_check(a == ws, "the first 'a' is at index 0");
	pc_check(b == ws + 1, "the first 'b' is at index 1");
	pc_check(c == ws + 2, "the first 'c' is at index 2");
	pc_pass();
}
