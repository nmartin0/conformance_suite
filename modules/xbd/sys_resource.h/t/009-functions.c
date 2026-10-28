/*
 * <sys/resource.h> functions
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sys_resource.h.009
 *
 * Only <sys/resource.h> is included (not pc.h, which brings in <stddef.h>), so a name
 * the header fails to provide stops the build, which the module reports as FAIL.
 */
#include <sys/resource.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

int main(void)
{
	pc_begin("XBD.sys_resource.h.009");
	pc_test();
	if (!IS_T(&(getrlimit), int (*)(int, struct rlimit *)))
		pc_fail("getrlimit() is not declared as int getrlimit(int, struct rlimit *)");
	if (IS_T(&(getrlimit), int (*)(int, struct rlimit *, int)))
		pc_fail("getrlimit() is declared without a prototype");
	if (!IS_T(&(setrlimit), int (*)(int, const struct rlimit *)))
		pc_fail("setrlimit() is not declared as int setrlimit(int, const struct rlimit *)");
	if (IS_T(&(setrlimit), int (*)(int, const struct rlimit *, int)))
		pc_fail("setrlimit() is declared without a prototype");
	pc_pass();
}
