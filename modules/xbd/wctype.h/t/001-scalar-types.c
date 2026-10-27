/*
 * wctype_t and wctrans_t are scalar types; wint_t is an integer type.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.wctype.h.001
 */
#include <wctype.h>
#include "pc.h"

int main(void)
{
	wctype_t t = (wctype_t)0;
	wctrans_t r = (wctrans_t)0;

	pc_begin("XBD.wctype.h.001");
	pc_test();
	pc_check(!t, "a zero wctype_t is true");
	pc_check(!r, "a zero wctrans_t is true");
	pc_check((wint_t)1.9 == 1, "wint_t is not an integer type");
	pc_pass();
}
