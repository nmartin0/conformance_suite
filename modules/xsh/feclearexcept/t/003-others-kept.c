/*
 * feclearexcept() leaves other exceptions alone.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.feclearexcept.003
 */
#include "fh.h"

int main(void)
{
	pc_begin("XSH.feclearexcept.003");
	fh_pair();
	pc_test();
	fh_raise(fh_e1);
	fh_raise(fh_e2);
	(void)feclearexcept(fh_e1);
	pc_check((fetestexcept(fh_e2) & fh_e2) != 0, "exception 0x%x was cleared by feclearexcept(0x%x)", fh_e2, fh_e1);
	fh_clearall();
	pc_pass();
}
