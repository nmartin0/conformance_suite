/*
 * The SA_ flags are distinct single bits usable together in sa_flags.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.signal.h.016
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
	pc_begin("XBD.signal.h.016");
	pc_test();
#if !defined(SA_NOCLDSTOP) || !defined(SA_RESETHAND) || !defined(SA_RESTART) || !defined(SA_SIGINFO) || !defined(SA_NODEFER)
	pc_fail("<signal.h> does not define every one of: SA_NOCLDSTOP SA_RESETHAND SA_RESTART SA_SIGINFO SA_NODEFER");
#else
	{
		_Static_assert((SA_NOCLDSTOP) == (SA_NOCLDSTOP), "SA_NOCLDSTOP is an integer constant expression");
		_Static_assert((SA_RESETHAND) == (SA_RESETHAND), "SA_RESETHAND is an integer constant expression");
		_Static_assert((SA_RESTART) == (SA_RESTART), "SA_RESTART is an integer constant expression");
		_Static_assert((SA_SIGINFO) == (SA_SIGINFO), "SA_SIGINFO is an integer constant expression");
		_Static_assert((SA_NODEFER) == (SA_NODEFER), "SA_NODEFER is an integer constant expression");
#define E_SA_NOCLDSTOP (SA_NOCLDSTOP)
#define E_SA_RESETHAND (SA_RESETHAND)
#define E_SA_RESTART (SA_RESTART)
#define E_SA_SIGINFO (SA_SIGINFO)
#define E_SA_NODEFER (SA_NODEFER)
		static const long v[] = { E_SA_NOCLDSTOP, E_SA_RESETHAND, E_SA_RESTART, E_SA_SIGINFO, E_SA_NODEFER };
		static const char *const name[] = { "SA_NOCLDSTOP", "SA_RESETHAND", "SA_RESTART", "SA_SIGINFO", "SA_NODEFER" };
		int i, j, n = (int)(sizeof v / sizeof v[0]);

		for (i = 0; i < n; i++) {
			pc_check(v[i] != 0, "%s is 0", name[i]);
			for (j = i + 1; j < n; j++)
				pc_check((v[i] & v[j]) == 0, "%s and %s have bits in common", name[i], name[j]);
		}
	}
	pc_pass();
#endif
}
