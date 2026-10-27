/*
 * wcsrchr() returns the last occurrence of the code.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcsrchr.001
 */
#include "pc.h"
#include <wchar.h>

static wchar_t ws[] = { L'a', L'b', L'c', L'a', L'b', L'c', 0 };
static wchar_t one[] = { L'a', L'b', L'c', 0 };

int main(void)
{
	wchar_t *a, *b, *c, *o;

	pc_begin("XSH.wcsrchr.001");
	pc_test();
	a = wcsrchr(ws, L'a');
	b = wcsrchr(ws, L'b');
	c = wcsrchr(ws, L'c');
	o = wcsrchr(one, L'b');
	pc_check(a == ws + 3, "the last 'a' is at index 3");
	pc_check(b == ws + 4, "the last 'b' is at index 4");
	pc_check(c == ws + 5, "the last 'c' is at index 5");
	pc_check(o == one + 1, "the only 'b' in \"abc\" is at index 1");
	pc_pass();
}
