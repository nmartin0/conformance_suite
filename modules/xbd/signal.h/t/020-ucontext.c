/*
 * ucontext_t has uc_link, uc_sigmask, uc_stack and uc_mcontext.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.signal.h.020
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
	pc_begin("XBD.signal.h.020");
	pc_test();
	{
		ucontext_t uc;
		mcontext_t *mc = &uc.uc_mcontext;	/* mcontext_t may be any object type */

		(void)mc;
		pc_check(IS_T(uc.uc_stack, stack_t), "uc_stack is not a stack_t");
		pc_check(IS_T(uc.uc_link, ucontext_t *), "uc_link is not a ucontext_t *");
		pc_check(sigemptyset(&uc.uc_sigmask) == 0, "uc_sigmask is not usable as a sigset_t");
	}
	pc_pass();
}
