/*
 * For csin, in the page's notation w stands for the product i*z, and the
 * value given is the factor that -i multiplies; the call is on z = -i*w
 * (parts exchanged, one sign changed) and the result is -i times that
 * factor. w = x + iInf (x positive finite: 1, 2.5) gives NaN + iNaN and the
 * invalid exception is raised.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.csin.004
 */
#include "mt.h"

CT_WRAP_CC(csin)

static const struct row rows[] = {
	{ X_V(1.0L), X_PINF, X_NAN, X_NAN, FL_INV },
	{ X_V(2.5L), X_PINF, X_NAN, X_NAN, FL_INV },
};

int main(void)
{
	pc_begin("XSH.csin.004");
	MT_NEED_MXC();
	pc_test();
	ct_run("csin", w_csin, rows, (int)(sizeof rows / sizeof rows[0]), 1, 0);
	pc_pass();
}
