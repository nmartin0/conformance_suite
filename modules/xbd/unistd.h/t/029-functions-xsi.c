/*
 * <unistd.h> functions xsi
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.unistd.h.029
 *
 * Only <unistd.h> is included (not pc.h, which brings in <stddef.h>), so a name
 * the header fails to provide stops the build, which the module reports as FAIL.
 */
#include <unistd.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

int main(void)
{
	pc_begin("XBD.unistd.h.029");
	pc_test();
	if (!IS_T(&(gethostid), long (*)(void)))
		pc_fail("gethostid() is not declared as long gethostid(void)");
	if (IS_T(&(gethostid), long (*)(int)))
		pc_fail("gethostid() is declared without a prototype");
	if (!IS_T(&(lockf), int (*)(int, int, off_t)))
		pc_fail("lockf() is not declared as int lockf(int, int, off_t)");
	if (IS_T(&(lockf), int (*)(int, int, off_t, int)))
		pc_fail("lockf() is declared without a prototype");
	if (!IS_T(&(nice), int (*)(int)))
		pc_fail("nice() is not declared as int nice(int)");
	if (IS_T(&(nice), int (*)(int, int)))
		pc_fail("nice() is declared without a prototype");
	if (!IS_T(&(setregid), int (*)(gid_t, gid_t)))
		pc_fail("setregid() is not declared as int setregid(gid_t, gid_t)");
	if (IS_T(&(setregid), int (*)(gid_t, gid_t, int)))
		pc_fail("setregid() is declared without a prototype");
	if (!IS_T(&(setreuid), int (*)(uid_t, uid_t)))
		pc_fail("setreuid() is not declared as int setreuid(uid_t, uid_t)");
	if (IS_T(&(setreuid), int (*)(uid_t, uid_t, int)))
		pc_fail("setreuid() is declared without a prototype");
	if (!IS_T(&(swab), void (*)(const void *restrict, void *restrict, ssize_t)))
		pc_fail("swab() is not declared as void swab(const void *restrict, void *restrict, ssize_t)");
	if (IS_T(&(swab), void (*)(const void *restrict, void *restrict, ssize_t, int)))
		pc_fail("swab() is declared without a prototype");
	if (!IS_T(&(sync), void (*)(void)))
		pc_fail("sync() is not declared as void sync(void)");
	if (IS_T(&(sync), void (*)(int)))
		pc_fail("sync() is declared without a prototype");
	pc_pass();
}
