/*
 * <sys/mman.h> functions mlr
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sys_mman.h.017
 *
 * Only <sys/mman.h> is included (not pc.h, which brings in <stddef.h>), so a name
 * the header fails to provide stops the build, which the module reports as FAIL.
 */
#include <sys/mman.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

int main(void)
{
	pc_begin("XBD.sys_mman.h.017");
	pc_test();
	if (!IS_T(&(mlock), int (*)(const void *, size_t)))
		pc_fail("mlock() is not declared as int mlock(const void *, size_t)");
	if (IS_T(&(mlock), int (*)(const void *, size_t, int)))
		pc_fail("mlock() is declared without a prototype");
	if (!IS_T(&(munlock), int (*)(const void *, size_t)))
		pc_fail("munlock() is not declared as int munlock(const void *, size_t)");
	if (IS_T(&(munlock), int (*)(const void *, size_t, int)))
		pc_fail("munlock() is declared without a prototype");
	pc_pass();
}
