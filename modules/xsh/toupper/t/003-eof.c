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
		/* control: a letter is converted, so EOF coming back as EOF is not the answer of a function that returns -1 */
		pc_check(cv_call(1, via, (unsigned char)ct_lower[0], (locale_t)0) == (unsigned char)ct_upper[0],
		    "toupper (%s) of the first small letter did not return the capital letter", cv_via[via]);
	for (via = 0; via <= 1; via++)
		pc_check(cv_call(1, via, EOF, (locale_t)0) == EOF, "toupper (%s) of EOF did not return EOF", cv_via[via]);
	pc_pass();
}
