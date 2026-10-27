/*
 * fetestexcept() reports only the raised exceptions among those queried.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fetestexcept.003
 */
#include "fh.h"

int main(void)
{
	int r;

	pc_begin("XSH.fetestexcept.003");
	fh_pair();
	fh_clearall();
	fh_raise(fh_e1);
	pc_test();
	r = fetestexcept(fh_e1 | fh_e2);
	pc_check(r == fh_e1, "fetestexcept(0x%x|0x%x) returned 0x%x, expected 0x%x", fh_e1, fh_e2, r, fh_e1);
	r = fetestexcept(fh_e2);
	pc_check(r == 0, "fetestexcept(0x%x) returned 0x%x with that exception clear", fh_e2, r);
	fh_clearall();
	pc_pass();
}
