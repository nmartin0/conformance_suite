/*
 * wcstok(): a last token running to the end, and strings with no token.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcstok.003
 */
#include "pc.h"
#include <wchar.h>

static wchar_t s[] = L"x;y", empty[1], seps[] = L";;;", sep[] = L";";

int main(void)
{
	wchar_t *r[4], *e, *f, *sv = NULL, *se = NULL, *sf = NULL;
	int i;

	pc_begin("XSH.wcstok.003");
	pc_test();
	r[0] = wcstok(s, sep, &sv);
	for (i = 1; i < 4; i++)
		r[i] = wcstok(NULL, sep, &sv);
	e = wcstok(empty, sep, &se);
	f = wcstok(seps, sep, &sf);
	pc_check(r[0] == s && wcscmp(s, L"x") == 0, "the first call did not return \"x\" at s");
	pc_check(r[1] == s + 2 && wcscmp(s + 2, L"y") == 0, "the second call did not return \"y\" at s+2");
	pc_check(r[2] == NULL, "the call after the last token did not return a null pointer");
	pc_check(r[3] == NULL, "the next call did not return a null pointer");
	pc_check(e == NULL, "wcstok() of \"\" did not return a null pointer");
	pc_check(f == NULL, "wcstok() of \";;;\" did not return a null pointer");
	pc_pass();
}
