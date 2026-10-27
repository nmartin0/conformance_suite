/*
 * feupdateenv(FE_DFL_ENV) returns zero.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.feupdateenv.003
 */
#include "fh.h"

int main(void)
{
	int r;

	pc_begin("XSH.feupdateenv.003");
	pc_test();
	r = feupdateenv(FE_DFL_ENV);
	pc_check(r == 0, "feupdateenv(FE_DFL_ENV) returned %d, expected 0", r);
	fh_clearall();
	pc_pass();
}
