/*
 * A wcstok() call with a non-null first argument ignores the old save pointer.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcstok.007
 */
#include "pc.h"
#include <wchar.h>

static wchar_t s[] = L"p,q", other[] = L"zz,zz", sep[] = L",";

int main(void)
{
	wchar_t *save, *r[3];

	pc_begin("XSH.wcstok.007");
	save = other;
	pc_test();
	r[0] = wcstok(s, sep, &save);
	r[1] = wcstok(NULL, sep, &save);
	r[2] = wcstok(NULL, sep, &save);
	pc_check(r[0] == s, "the first call did not return s");
	pc_check(r[1] == s + 2, "the second call did not return s+2");
	pc_check(r[2] == NULL, "the third call did not return a null pointer");
	pc_pass();
}
