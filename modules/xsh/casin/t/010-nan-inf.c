/*
 * For casin, in the page's notation w stands for the product i*z, and the
 * value given is the factor that -i multiplies; the call is on z = -i*w
 * (parts exchanged, one sign changed) and the result is -i times that
 * factor. w = NaN + iInf gives an infinite real part of either sign and a
 * NaN imaginary part.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.casin.010
 */
#include "mt.h"

CT_WRAP_CC(casin)

static const struct row rows[] = {
	{ X_NAN, X_PINF, X_INF, X_NAN, 0 },
};

int main(void)
{
	pc_begin("XSH.casin.010");
	MT_NEED_MXC();
	pc_test();
	ct_run("casin", w_casin, rows, (int)(sizeof rows / sizeof rows[0]), 1, 0);
	pc_pass();
}
