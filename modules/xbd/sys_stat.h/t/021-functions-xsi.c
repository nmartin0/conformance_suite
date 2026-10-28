/*
 * <sys/stat.h> functions xsi
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sys_stat.h.021
 *
 * Only <sys/stat.h> is included (not pc.h, which brings in <stddef.h>), so a name
 * the header fails to provide stops the build, which the module reports as FAIL.
 */
#include <sys/stat.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

int main(void)
{
	pc_begin("XBD.sys_stat.h.021");
	pc_test();
	if (!IS_T(&(mknod), int (*)(const char *, mode_t, dev_t)))
		pc_fail("mknod() is not declared as int mknod(const char *, mode_t, dev_t)");
	if (IS_T(&(mknod), int (*)(const char *, mode_t, dev_t, int)))
		pc_fail("mknod() is declared without a prototype");
	if (!IS_T(&(mknodat), int (*)(int, const char *, mode_t, dev_t)))
		pc_fail("mknodat() is not declared as int mknodat(int, const char *, mode_t, dev_t)");
	if (IS_T(&(mknodat), int (*)(int, const char *, mode_t, dev_t, int)))
		pc_fail("mknodat() is declared without a prototype");
	pc_pass();
}
