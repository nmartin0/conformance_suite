/*
 * wcsrchr() treats the terminating null code as part of the string.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcsrchr.002
 */
#include "pc.h"
#include <wchar.h>

static wchar_t ws[] = { L'a', L'b', L'c', 0 }, empty[] = { 0 };

int main(void)
{
	wchar_t *r, *e;

	pc_begin("XSH.wcsrchr.002");
	pc_test();
	r = wcsrchr(ws, 0);
	e = wcsrchr(empty, 0);
	pc_check(r == ws + 3, "wcsrchr(\"abc\", 0) did not return a pointer to the terminator");
	pc_check(e == empty, "wcsrchr(\"\", 0) did not return its first argument");
	pc_pass();
}
