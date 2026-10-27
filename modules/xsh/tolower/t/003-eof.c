/*
 * tolower: EOF
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.tolower.003
 */
#include "cv.h"

int main(void)
{
	int via;

	pc_begin("XSH.tolower.003");
	ct_init();
	pc_test();
	for (via = 0; via <= 1; via++)
		pc_check(cv_call(0, via, EOF, (locale_t)0) == EOF, "tolower (%s) of EOF did not return EOF", cv_via[via]);
	pc_pass();
}
