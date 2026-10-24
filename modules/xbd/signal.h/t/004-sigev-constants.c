/*
 * SIGEV_NONE, SIGEV_SIGNAL and SIGEV_THREAD are distinct constants.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.signal.h.004
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
	pc_begin("XBD.signal.h.004");
	pc_test();
#if !defined(SIGEV_NONE) || !defined(SIGEV_SIGNAL) || !defined(SIGEV_THREAD)
	pc_fail("<signal.h> does not define SIGEV_NONE, SIGEV_SIGNAL and SIGEV_THREAD as macros");
#else
	{
		_Static_assert((SIGEV_NONE) == (SIGEV_NONE), "SIGEV_NONE is an integer constant expression");
		_Static_assert((SIGEV_SIGNAL) == (SIGEV_SIGNAL), "SIGEV_SIGNAL is an integer constant expression");
		_Static_assert((SIGEV_THREAD) == (SIGEV_THREAD), "SIGEV_THREAD is an integer constant expression");
#define N (SIGEV_NONE)
#define S (SIGEV_SIGNAL)
#define T (SIGEV_THREAD)
		static const long v[] = { N, S, T };
		static const char *const name[] = { "SIGEV_NONE", "SIGEV_SIGNAL", "SIGEV_THREAD" };
		struct sigevent ev;

		ev.sigev_notify = SIGEV_THREAD;
		pc_check(ev.sigev_notify == SIGEV_THREAD, "SIGEV_THREAD does not fit in sigev_notify");
		distinct("sigev_notify values", v, name, 3);
	}
	pc_pass();
#endif
}
