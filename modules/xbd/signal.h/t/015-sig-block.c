/*
 * SIG_BLOCK, SIG_UNBLOCK and SIG_SETMASK are distinct integer constants.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.signal.h.015
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
	pc_begin("XBD.signal.h.015");
	pc_test();
#if !defined(SIG_BLOCK) || !defined(SIG_UNBLOCK) || !defined(SIG_SETMASK)
	pc_fail("<signal.h> does not define SIG_BLOCK, SIG_UNBLOCK and SIG_SETMASK as macros");
#else
	{
		enum { B = SIG_BLOCK, U = SIG_UNBLOCK, S = SIG_SETMASK };
		static const long v[] = { B, U, S };
		static const char *const name[] = { "SIG_BLOCK", "SIG_UNBLOCK", "SIG_SETMASK" };

		distinct("sigprocmask how values", v, name, 3);
	}
	pc_pass();
#endif
}
