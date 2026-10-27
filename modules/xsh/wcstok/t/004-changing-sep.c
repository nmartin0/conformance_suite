/*
 * wcstok() uses the separators given on each call.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcstok.004
 */
#include "pc.h"
#include <wchar.h>

static wchar_t s[] = L"a,b c,d", comma[] = L",", space[] = L" ";

int main(void)
{
	wchar_t *r[5], *save = NULL;

	pc_begin("XSH.wcstok.004");
	pc_test();
	r[0] = wcstok(s, comma, &save);
	r[1] = wcstok(NULL, space, &save);
	r[2] = wcstok(NULL, comma, &save);
	r[3] = wcstok(NULL, comma, &save);
	r[4] = wcstok(NULL, comma, &save);
	pc_check(r[0] == s && wcscmp(r[0], L"a") == 0, "the first call did not return \"a\" at s");
	pc_check(r[1] == s + 2 && wcscmp(r[1], L"b") == 0, "the call with \" \" did not return \"b\" at s+2");
	pc_check(r[2] == s + 4 && wcscmp(r[2], L"c") == 0, "the third call did not return \"c\" at s+4");
	pc_check(r[3] == s + 6 && wcscmp(r[3], L"d") == 0, "the fourth call did not return \"d\" at s+6");
	pc_check(r[4] == NULL, "the fifth call did not return a null pointer");
	pc_pass();
}
