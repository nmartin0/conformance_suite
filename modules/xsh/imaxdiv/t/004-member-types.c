/*
 * imaxdiv: member types.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.imaxdiv.004
 */
#include "pc.h"
#include <inttypes.h>
static imaxdiv_t (*volatile f_imaxdiv)(intmax_t, intmax_t) = imaxdiv;

int main(void)
{
	imaxdiv_t r;
	pc_begin("XSH.imaxdiv.004");
	pc_test();
	r = f_imaxdiv(9, 4);
	pc_check(_Generic(r.quot, intmax_t: 1, default: 0), "quot does not have type intmax_t");
	pc_check(_Generic(r.rem, intmax_t: 1, default: 0), "rem does not have type intmax_t");
	pc_check(r.quot == 2 && r.rem == 1, "(9, 4) was wrong");
	pc_pass();
}
