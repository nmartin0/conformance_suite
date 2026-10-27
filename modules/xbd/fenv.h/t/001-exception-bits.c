/*
 * The FE_ exception macros that are defined are integer constant
 *   expressions with no bits in common.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.fenv.h.001
 */
#include <fenv.h>
#include "pc.h"

static const struct { const char *name; int value; } tab[] = {
#ifdef FE_DIVBYZERO
	{ "FE_DIVBYZERO", FE_DIVBYZERO },
#endif
#ifdef FE_INEXACT
	{ "FE_INEXACT", FE_INEXACT },
#endif
#ifdef FE_INVALID
	{ "FE_INVALID", FE_INVALID },
#endif
#ifdef FE_OVERFLOW
	{ "FE_OVERFLOW", FE_OVERFLOW },
#endif
#ifdef FE_UNDERFLOW
	{ "FE_UNDERFLOW", FE_UNDERFLOW },
#endif
	{ "", 0 }
};
enum {
#ifdef FE_DIVBYZERO
	ICE_FE_DIVBYZERO = FE_DIVBYZERO,
#endif
#ifdef FE_INEXACT
	ICE_FE_INEXACT = FE_INEXACT,
#endif
#ifdef FE_INVALID
	ICE_FE_INVALID = FE_INVALID,
#endif
#ifdef FE_OVERFLOW
	ICE_FE_OVERFLOW = FE_OVERFLOW,
#endif
#ifdef FE_UNDERFLOW
	ICE_FE_UNDERFLOW = FE_UNDERFLOW,
#endif
	ICE_END
};

int main(void)
{
	size_t i, j, n = sizeof tab / sizeof tab[0] - 1;

	pc_begin("XBD.fenv.h.001");
	pc_test();
	if (n == 0)
		pc_notinuse("<fenv.h> defines no floating-point exception macro");
	for (i = 0; i < n; i++)
		for (j = i + 1; j < n; j++)
			pc_check((tab[i].value & tab[j].value) == 0, "%s (%#x) and %s (%#x) have bits in common",
			    tab[i].name, (unsigned)tab[i].value, tab[j].name, (unsigned)tab[j].value);
	pc_pass();
}
