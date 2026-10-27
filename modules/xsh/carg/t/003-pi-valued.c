/*
 * carg returns +-pi for -0 +- i0, x +- i0 (x negative) and -Inf +- iy (y
 * positive finite); +-pi/2 for +-0 + iy (y positive or negative) and x +-
 * iInf (x finite); +-3pi/4 for -Inf +- iInf; +-pi/4 for +Inf +- iInf, each
 * sign following the imaginary part.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.carg.003
 */
#include "mt.h"

CT_WRAP_CR(carg)

static const struct row rows[] = {
	{ X_N0, X_P0, X_Q(4), X_ANY, 0 },
	{ X_N0, X_N0, X_Q(-4), X_ANY, 0 },
	{ X_V(-2.5L), X_P0, X_Q(4), X_ANY, 0 },
	{ X_V(-2.5L), X_N0, X_Q(-4), X_ANY, 0 },
	{ X_V(-1.0L), X_P0, X_Q(4), X_ANY, 0 },
	{ X_V(-1.0L), X_N0, X_Q(-4), X_ANY, 0 },
	{ X_P0, X_V(1.0L), X_Q(2), X_ANY, 0 },
	{ X_N0, X_V(1.0L), X_Q(2), X_ANY, 0 },
	{ X_P0, X_V(2.5L), X_Q(2), X_ANY, 0 },
	{ X_P0, X_V(-1.0L), X_Q(-2), X_ANY, 0 },
	{ X_N0, X_V(-1.0L), X_Q(-2), X_ANY, 0 },
	{ X_P0, X_V(-2.5L), X_Q(-2), X_ANY, 0 },
	{ X_NINF, X_V(1.0L), X_Q(4), X_ANY, 0 },
	{ X_NINF, X_V(-1.0L), X_Q(-4), X_ANY, 0 },
	{ X_NINF, X_V(2.5L), X_Q(4), X_ANY, 0 },
	{ X_NINF, X_V(-2.5L), X_Q(-4), X_ANY, 0 },
	{ X_V(1.0L), X_PINF, X_Q(2), X_ANY, 0 },
	{ X_V(-1.0L), X_PINF, X_Q(2), X_ANY, 0 },
	{ X_P0, X_PINF, X_Q(2), X_ANY, 0 },
	{ X_N0, X_PINF, X_Q(2), X_ANY, 0 },
	{ X_V(1.0L), X_NINF, X_Q(-2), X_ANY, 0 },
	{ X_V(-1.0L), X_NINF, X_Q(-2), X_ANY, 0 },
	{ X_P0, X_NINF, X_Q(-2), X_ANY, 0 },
	{ X_N0, X_NINF, X_Q(-2), X_ANY, 0 },
	{ X_NINF, X_PINF, X_Q(3), X_ANY, 0 },
	{ X_NINF, X_NINF, X_Q(-3), X_ANY, 0 },
	{ X_PINF, X_PINF, X_Q(1), X_ANY, 0 },
	{ X_PINF, X_NINF, X_Q(-1), X_ANY, 0 },
};

int main(void)
{
	pc_begin("XSH.carg.003");
	MT_NEED_MXC();
	pc_test();
	ct_run("carg", w_carg, rows, (int)(sizeof rows / sizeof rows[0]), 0, 1);
	pc_pass();
}
