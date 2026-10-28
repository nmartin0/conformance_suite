/*
 * <dlfcn.h> functions
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.dlfcn.h.004
 *
 * Only <dlfcn.h> is included (not pc.h, which brings in <stddef.h>), so a name
 * the header fails to provide stops the build, which the module reports as FAIL.
 */
#include <dlfcn.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

int main(void)
{
	pc_begin("XBD.dlfcn.h.004");
	pc_test();
	if (!IS_T(&(dladdr), int (*)(const void *restrict, Dl_info_t *restrict)))
		pc_fail("dladdr() is not declared as int dladdr(const void *restrict, Dl_info_t *restrict)");
	if (IS_T(&(dladdr), int (*)(const void *restrict, Dl_info_t *restrict, int)))
		pc_fail("dladdr() is declared without a prototype");
	pc_pass();
}
