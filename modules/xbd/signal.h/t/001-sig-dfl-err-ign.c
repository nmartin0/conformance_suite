/*
 * SIG_DFL, SIG_ERR and SIG_IGN are distinct handler constants.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.signal.h.001
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

static void some_handler(int sig)
{
	(void)sig;
}

int main(void)
{
	pc_begin("XBD.signal.h.001");
	pc_test();
#if !defined(SIG_DFL) || !defined(SIG_ERR) || !defined(SIG_IGN)
	pc_fail("<signal.h> does not define SIG_DFL, SIG_ERR and SIG_IGN as macros");
#else
	{
		typedef void (*handler)(int);
		static const handler k[3] = { SIG_DFL, SIG_ERR, SIG_IGN };
		handler fn = some_handler, r;

		pc_check(IS_T(SIG_DFL, handler) && IS_T(SIG_ERR, handler) && IS_T(SIG_IGN, handler),
		    "SIG_DFL, SIG_ERR or SIG_IGN does not have the type void (*)(int)");
		r = signal(SIGINT, k[0]);	/* the same type as signal()'s argument and result */
		pc_check(r != SIG_ERR, "signal(SIGINT, SIG_DFL) failed");
		pc_check(k[0] != k[1] && k[0] != k[2] && k[1] != k[2], "SIG_DFL, SIG_ERR and SIG_IGN are not all different");
		pc_check(k[0] != fn && k[1] != fn && k[2] != fn, "SIG_DFL, SIG_ERR or SIG_IGN equals the address of a function");
	}
	pc_pass();
#endif
}
