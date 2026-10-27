/*
 * wcstok() returns each token in turn, then null pointers.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcstok.001
 */
#include "pc.h"
#include <wchar.h>

static wchar_t s[] = L"  ab,,cd e  ", sep[] = L" ,";

int main(void)
{
	wchar_t *r[5], *save = NULL;
	int i;

	pc_begin("XSH.wcstok.001");
	pc_test();
	r[0] = wcstok(s, sep, &save);
	for (i = 1; i < 5; i++)
		r[i] = wcstok(NULL, sep, &save);
	pc_check(r[0] == s + 2, "the first call did not return s+2");
	pc_check(wcscmp(r[0], L"ab") == 0, "the first token is not \"ab\"");
	pc_check(r[1] == s + 6, "the second call did not return s+6");
	pc_check(wcscmp(r[1], L"cd") == 0, "the second token is not \"cd\"");
	pc_check(r[2] == s + 9, "the third call did not return s+9");
	pc_check(wcscmp(r[2], L"e") == 0, "the third token is not \"e\"");
	pc_check(r[3] == NULL, "the fourth call (only separators left) did not return a null pointer");
	pc_check(r[4] == NULL, "the fifth call did not return a null pointer");
	pc_pass();
}
