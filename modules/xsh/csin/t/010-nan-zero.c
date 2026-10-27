/*
 * For csin, in the page's notation w stands for the product i*z, and the
 * value given is the factor that -i multiplies; the call is on z = -i*w
 * (parts exchanged, one sign changed) and the result is -i times that
 * factor. w = NaN + i0 gives NaN + i0.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.csin.010
 */
#include "mt.h"

CT_WRAP_CC(csin)

static const struct row rows[] = {
	{ X_NAN, X_P0, X_NAN, X_P0, 0 },
};

int main(void)
{
	pc_begin("XSH.csin.010");
	MT_NEED_MXC();
	pc_test();
	ct_run("csin", w_csin, rows, (int)(sizeof rows / sizeof rows[0]), 1, 0);
	pc_pass();
}
