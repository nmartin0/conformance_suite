/*
 * <string.h> functions xsi
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.string.h.004
 *
 * Only <string.h> is included (not pc.h, which brings in <stddef.h>), so a name
 * the header fails to provide stops the build, which the module reports as FAIL.
 */
#include <string.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

int main(void)
{
	pc_begin("XBD.string.h.004");
	pc_test();
	if (!IS_T(&(memccpy), void *(*)(void *, const void *, int, size_t)))
		pc_fail("memccpy() is not declared as void *memccpy(void *restrict, const void *restrict, int, size_t)");
	if (IS_T(&(memccpy), void *(*)(void *, const void *, int, size_t, int)))
		pc_fail("memccpy() is declared without a prototype");
	pc_pass();
}
