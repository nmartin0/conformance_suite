/*
 * The duplicate made by wcsdup() is independent of the original.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcsdup.002
 */
#include "pc.h"
#include <stdlib.h>
#include <wchar.h>

static wchar_t s[] = { L'a', L'b', L'c', 0 };

int main(void)
{
	wchar_t *d;

	pc_begin("XSH.wcsdup.002");
	pc_test();
	d = wcsdup(s);
	if (d == NULL)
		pc_unresolved("wcsdup() returned a null pointer (memory may have run out)");
	s[0] = L'z';
	pc_check(d[0] == L'a', "changing the original changed the duplicate");
	d[1] = L'y';
	pc_check(s[1] == L'b', "changing the duplicate changed the original");
	free(d);
	pc_pass();
}
