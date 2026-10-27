/*
 * wcstok() with an empty separator string returns the whole string.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcstok.005
 */
#include "pc.h"
#include <wchar.h>

static wchar_t s[] = L" ab c", sep[1];

int main(void)
{
	wchar_t *r0, *r1, *save = NULL;

	pc_begin("XSH.wcstok.005");
	pc_test();
	r0 = wcstok(s, sep, &save);
	r1 = wcstok(NULL, sep, &save);
	pc_check(r0 == s, "the first call did not return s");
	pc_check(wcscmp(s, L" ab c") == 0, "the string was changed");
	pc_check(r1 == NULL, "the second call did not return a null pointer");
	pc_pass();
}
