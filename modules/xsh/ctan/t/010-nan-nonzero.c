/*
 * For ctan, in the page's notation w stands for the product i*z, and the
 * value given is the factor that -i multiplies; the call is on z = -i*w
 * (parts exchanged, one sign changed) and the result is -i times that
 * factor. w = NaN + iy (y non-zero finite) and w = NaN + iNaN give NaN +
 * iNaN.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.ctan.010
 */
#include "mt.h"

CT_WRAP_CC(ctan)

static const struct row rows[] = {
	{ X_NAN, X_V(1.0L), X_NAN, X_NAN, 0 },
	{ X_NAN, X_V(-1.0L), X_NAN, X_NAN, 0 },
	{ X_NAN, X_NAN, X_NAN, X_NAN, 0 },
};

int main(void)
{
	pc_begin("XSH.ctan.010");
	MT_NEED_MXC();
	pc_test();
	ct_run("ctan", w_ctan, rows, (int)(sizeof rows / sizeof rows[0]), 1, 0);
	pc_pass();
}
