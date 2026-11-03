/*
 * <search.h> ENTRY members
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.search.h.001
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
	pc_begin("XBD.search.h.001");
	pc_test();
	ENTRY e;

	e.key = (char *)0;
	e.data = (void *)0;
	if (!IS_T(e.key, char *))
		pc_fail("ENTRY member key does not have type char *");
	if (!IS_T(e.data, void *))
		pc_fail("ENTRY member data does not have type void *");
	pc_pass();
}
