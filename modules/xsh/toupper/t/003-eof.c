/*
 * toupper: EOF
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.toupper.003
 */
#include "cv.h"

int main(void)
{
	int via;

	pc_begin("XSH.toupper.003");
	ct_init();
	pc_test();
	for (via = 0; via <= 1; via++)
		pc_check(cv_call(1, via, EOF, (locale_t)0) == EOF, "toupper (%s) of EOF did not return EOF", cv_via[via]);
	pc_pass();
}
