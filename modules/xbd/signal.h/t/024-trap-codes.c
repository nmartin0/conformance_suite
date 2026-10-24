/*
 * TRAP_BRKPT and TRAP_TRACE are defined and distinct.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.signal.h.024
 *
 * Only <signal.h> is included (pc.h brings in <stddef.h> alone), so a
 * type or name this test uses that the header does not provide stops
 * the build, and the test is UNINITIATED.
 */
#include <signal.h>
#include "pc.h"

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

/* Fails unless the n values in v are all different. */
static void distinct(const char *what, const long *v, const char *const *name, int n)
{
	int i, j;

	for (i = 0; i < n; i++)
		for (j = i + 1; j < n; j++)
			pc_check(v[i] != v[j], "%s: %s and %s are both %ld", what, name[i], name[j], v[i]);
}

int main(void)
{
	pc_begin("XBD.signal.h.024");
	pc_test();
#if !defined(TRAP_BRKPT) || !defined(TRAP_TRACE)
	pc_fail("<signal.h> does not define every si_code constant: missing one of TRAP_BRKPT TRAP_TRACE");
#else
	{
		_Static_assert((TRAP_BRKPT) == (TRAP_BRKPT), "TRAP_BRKPT is an integer constant expression");
		_Static_assert((TRAP_TRACE) == (TRAP_TRACE), "TRAP_TRACE is an integer constant expression");
#define E_TRAP_BRKPT (TRAP_BRKPT)
#define E_TRAP_TRACE (TRAP_TRACE)
		static const long v[] = { E_TRAP_BRKPT, E_TRAP_TRACE };
		static const char *const name[] = { "TRAP_BRKPT", "TRAP_TRACE" };
		distinct("si_code values for SIGTRAP", v, name, 2);
	}
	pc_pass();
#endif
}
