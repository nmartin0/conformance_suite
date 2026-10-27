/*
 * The FE_ rounding macros that are defined are integer constant
 *   expressions with distinct, non-negative values.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.fenv.h.003
 */
#include <fenv.h>
#include "pc.h"

static const struct { const char *name; int value; } tab[] = {
#ifdef FE_DOWNWARD
	{ "FE_DOWNWARD", FE_DOWNWARD },
#endif
#ifdef FE_TONEAREST
	{ "FE_TONEAREST", FE_TONEAREST },
#endif
#ifdef FE_TOWARDZERO
	{ "FE_TOWARDZERO", FE_TOWARDZERO },
#endif
#ifdef FE_UPWARD
	{ "FE_UPWARD", FE_UPWARD },
#endif
	{ "", 0 }
};
enum {
#ifdef FE_DOWNWARD
	ICE_FE_DOWNWARD = FE_DOWNWARD,
#endif
#ifdef FE_TONEAREST
	ICE_FE_TONEAREST = FE_TONEAREST,
#endif
#ifdef FE_TOWARDZERO
	ICE_FE_TOWARDZERO = FE_TOWARDZERO,
#endif
#ifdef FE_UPWARD
	ICE_FE_UPWARD = FE_UPWARD,
#endif
	ICE_END
};

int main(void)
{
	size_t i, j, n = sizeof tab / sizeof tab[0] - 1;

	pc_begin("XBD.fenv.h.003");
	pc_test();
	if (n == 0)
		pc_notinuse("<fenv.h> defines no rounding direction macro");
	for (i = 0; i < n; i++) {
		pc_check(tab[i].value >= 0, "%s is negative (%d)", tab[i].name, tab[i].value);
		for (j = i + 1; j < n; j++)
			pc_check(tab[i].value != tab[j].value, "%s and %s are both %d", tab[i].name, tab[j].name, tab[i].value);
	}
	pc_pass();
}
