/*
 * For casin, in the page's notation w stands for the product i*z, and the
 * value given is the factor that -i multiplies; the call is on z = -i*w
 * (parts exchanged, one sign changed) and the result is -i times that
 * factor. w = x + iInf (x positive finite: 1, 2.5) gives imaginary part
 * pi/2.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.casin.003
 */
#include "mt.h"

CT_WRAP_CC(casin)

static const struct row rows[] = {
	{ X_V(1.0L), X_PINF, X_PINF, X_Q(2), 0 },
	{ X_V(2.5L), X_PINF, X_PINF, X_Q(2), 0 },
};

int main(void)
{
	pc_begin("XSH.casin.003");
	MT_NEED_MXC();
	pc_test();
	ct_run("casin", w_casin, rows, (int)(sizeof rows / sizeof rows[0]), 1, 0);
	pc_pass();
}
