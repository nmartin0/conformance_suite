/*
 * Signedness and meaning of ptrdiff_t, size_t and wchar_t.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.stddef.h.003
 */
#include <stddef.h>
#include "pc.h"

_Static_assert((ptrdiff_t)-1 < 0, "ptrdiff_t is not signed");
_Static_assert((size_t)-1 > 0, "size_t is not unsigned");
_Static_assert(_Generic(sizeof(int), size_t: 1, default: 0), "sizeof does not have type size_t");
_Static_assert(_Generic((char *)0 - (char *)0, ptrdiff_t: 1, default: 0), "a pointer difference does not have type ptrdiff_t");
_Static_assert(L'\0' == 0, "the null wide character is not zero");
_Static_assert((wchar_t)1 % (wchar_t)1 == 0, "wchar_t is not an integer type");

int main(void)
{
	pc_begin("XBD.stddef.h.003");
	pc_test();
	int a[4];
	wchar_t w = L'\0';

	pc_check(&a[3] - &a[0] == 3 && &a[0] - &a[3] == -3, "a pointer difference is wrong");
	pc_check(w == 0, "wchar_t does not hold the null character");
	pc_pass();
}
