/*
 * FE_ALL_EXCEPT is an integer constant expression holding every defined
 *   exception macro.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.fenv.h.002
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

#ifdef FE_ALL_EXCEPT
enum { ICE_ALL = FE_ALL_EXCEPT };
#endif

int main(void)
{
	size_t i, n = sizeof tab / sizeof tab[0] - 1;

	pc_begin("XBD.fenv.h.002");
	pc_test();
#ifndef FE_ALL_EXCEPT
	pc_fail("<fenv.h> does not define FE_ALL_EXCEPT");
#else
	for (i = 0; i < n; i++)
		pc_check((FE_ALL_EXCEPT & tab[i].value) == tab[i].value, "FE_ALL_EXCEPT (%#x) lacks the bits of %s (%#x)",
		    (unsigned)FE_ALL_EXCEPT, tab[i].name, (unsigned)tab[i].value);
	pc_pass();
#endif
}
