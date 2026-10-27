/*
 * wcsrchr() returns a null pointer when the code is not in the string.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcsrchr.003
 */
#include "pc.h"
#include <wchar.h>

static wchar_t abc[] = { L'a', L'b', L'c', 0 }, empty[] = { 0 };
static wchar_t beyond[] = { L'a', L'b', 0, L'c', L'd', 0 };

int main(void)
{
	wchar_t *r[3];

	pc_begin("XSH.wcsrchr.003");
	pc_test();
	r[0] = wcsrchr(abc, L'z');
	r[1] = wcsrchr(empty, L'a');
	r[2] = wcsrchr(beyond, L'c');
	pc_check(r[0] == NULL, "'z' was reported in \"abc\"");
	pc_check(r[1] == NULL, "'a' was reported in an empty string");
	pc_check(r[2] == NULL, "a code after the null code was reported as part of the string");
	pc_pass();
}
