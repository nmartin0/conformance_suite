/*
 * struct sigaction has sa_handler, sa_mask and sa_flags.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.signal.h.013
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
	pc_begin("XBD.signal.h.013");
	pc_test();
	{
		struct sigaction sa;

		pc_check(IS_T(sa.sa_handler, void (*)(int)), "sa_handler is not void (*)(int)");
		pc_check(IS_T(sa.sa_flags, int), "sa_flags is not an int");
		pc_check(sigemptyset(&sa.sa_mask) == 0, "sa_mask is not usable as a sigset_t");
	}
	pc_pass();
}
