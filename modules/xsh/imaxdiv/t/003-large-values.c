/*
 * imaxdiv: large values.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.imaxdiv.003
 */
#include "pc.h"
#include <inttypes.h>
static imaxdiv_t (*volatile f_imaxdiv)(intmax_t, intmax_t) = imaxdiv;

int main(void)
{
	imaxdiv_t r;
	pc_begin("XSH.imaxdiv.003");
	pc_test();
	r = f_imaxdiv(5000000000LL, 3);
	pc_check(r.quot == 1666666666 && r.rem == 2, "(5000000000, 3) was wrong");
	r = f_imaxdiv(-5000000000LL, 3);
	pc_check(r.quot == -1666666666 && r.rem == -2, "(-5000000000, 3) was wrong");
	r = f_imaxdiv(INTMAX_MAX, 1);
	pc_check(r.quot == INTMAX_MAX && r.rem == 0, "(INTMAX_MAX, 1) was wrong");
	r = f_imaxdiv(INTMAX_MAX, INTMAX_MAX);
	pc_check(r.quot == 1 && r.rem == 0, "(INTMAX_MAX, INTMAX_MAX) was wrong");
	r = f_imaxdiv(-INTMAX_MAX, INTMAX_MAX);
	pc_check(r.quot == -1 && r.rem == 0, "(-INTMAX_MAX, INTMAX_MAX) was wrong");
	r = f_imaxdiv(INTMAX_MAX, 2);
	pc_check(r.quot == INTMAX_MAX / 2 && r.rem == 1, "(INTMAX_MAX, 2) was wrong");
	pc_pass();
}
