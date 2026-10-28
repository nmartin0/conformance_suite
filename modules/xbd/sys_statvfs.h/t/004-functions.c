/*
 * <sys/statvfs.h> functions
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sys_statvfs.h.004
 *
 * Only <sys/statvfs.h> is included (not pc.h, which brings in <stddef.h>), so a name
 * the header fails to provide stops the build, which the module reports as FAIL.
 */
#include <sys/statvfs.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

int main(void)
{
	pc_begin("XBD.sys_statvfs.h.004");
	pc_test();
	if (!IS_T(&(fstatvfs), int (*)(int, struct statvfs *)))
		pc_fail("fstatvfs() is not declared as int fstatvfs(int, struct statvfs *)");
	if (IS_T(&(fstatvfs), int (*)(int, struct statvfs *, int)))
		pc_fail("fstatvfs() is declared without a prototype");
	if (!IS_T(&(statvfs), int (*)(const char *restrict, struct statvfs *restrict)))
		pc_fail("statvfs() is not declared as int statvfs(const char *restrict, struct statvfs *restrict)");
	if (IS_T(&(statvfs), int (*)(const char *restrict, struct statvfs *restrict, int)))
		pc_fail("statvfs() is declared without a prototype");
	pc_pass();
}
