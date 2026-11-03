/*
 * <strings.h> functions xsi
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.strings.h.004
 *
 * Only <strings.h> is included (not pc.h, which brings in <stddef.h>), so a name
 * the header fails to provide stops the build, which the module reports as FAIL.
 */
#include <strings.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

int main(void)
{
	pc_begin("XBD.strings.h.004");
	pc_test();
	if (!IS_T(&(ffs), int (*)(int)))
		pc_fail("ffs() is not declared as int ffs(int)");
	if (IS_T(&(ffs), int (*)(int, int)))
		pc_fail("ffs() is declared without a prototype");
	if (!IS_T(&(ffsl), int (*)(long)))
		pc_fail("ffsl() is not declared as int ffsl(long)");
	if (IS_T(&(ffsl), int (*)(long, int)))
		pc_fail("ffsl() is declared without a prototype");
	if (!IS_T(&(ffsll), int (*)(long long)))
		pc_fail("ffsll() is not declared as int ffsll(long long)");
	if (IS_T(&(ffsll), int (*)(long long, int)))
		pc_fail("ffsll() is declared without a prototype");
	pc_pass();
}
