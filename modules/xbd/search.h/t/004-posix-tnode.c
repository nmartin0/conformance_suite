/*
 * <search.h> posix_tnode
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.search.h.004
 *
 * Only <search.h> is included (not pc.h, which brings in <stddef.h>), so a name
 * the header fails to provide stops the build, which the module reports as FAIL.
 */
#include <search.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

int main(void)
{
	pc_begin("XBD.search.h.004");
	pc_test();
	posix_tnode *t = (void *)0;
	void *p = t;

	if (!IS_T(t, void *))
		pc_fail("posix_tnode is not an alias for void");
	if (p != 0)
		pc_fail("a null posix_tnode pointer is not null");
	pc_pass();
}
