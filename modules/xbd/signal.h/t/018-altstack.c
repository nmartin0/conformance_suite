/*
 * SS_ONSTACK, SS_DISABLE, MINSIGSTKSZ and SIGSTKSZ are symbolic constants.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.signal.h.018
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
	pc_begin("XBD.signal.h.018");
	pc_test();
#if !defined(SS_ONSTACK) || !defined(SS_DISABLE) || !defined(MINSIGSTKSZ) || !defined(SIGSTKSZ)
	pc_fail("<signal.h> does not define SS_ONSTACK, SS_DISABLE, MINSIGSTKSZ and SIGSTKSZ");
#else
	{
		enum { O = SS_ONSTACK, D = SS_DISABLE, MN = MINSIGSTKSZ, DF = SIGSTKSZ };	/* integer constant expressions */

		pc_check(O != D, "SS_ONSTACK and SS_DISABLE are equal");
		pc_check(MN > 0 && DF > 0, "MINSIGSTKSZ is %ld and SIGSTKSZ %ld", (long)MN, (long)DF);
	}
	pc_pass();
#endif
}
