/*
 * <devctl.h> functions
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.devctl.h.002
 *
 * Only <devctl.h> is included (not pc.h, which brings in other headers), so a
 * name the header fails to provide stops the build, which the module
 * reports as FAIL.
 */
#include <devctl.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

int main(void)
{
	pc_begin("XBD.devctl.h.002");
	pc_test();
	if (!IS_T(&(posix_devctl), int (*)(int, int, void *, size_t, int *)))
		pc_fail("posix_devctl() is not declared with the prototype the page gives");
	pc_pass();
}
