/*
 * <unistd.h> functions issue8 xsi
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.unistd.h.025
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
	pc_begin("XBD.unistd.h.025");
	pc_test();
	if (!IS_T(&(getresgid), int (*)(gid_t *restrict, gid_t *restrict, gid_t *restrict)))
		pc_fail("getresgid() is not declared as int getresgid(gid_t *restrict, gid_t *restrict, gid_t *restrict)");
	if (IS_T(&(getresgid), int (*)(gid_t *restrict, gid_t *restrict, gid_t *restrict, int)))
		pc_fail("getresgid() is declared without a prototype");
	if (!IS_T(&(getresuid), int (*)(uid_t *restrict, uid_t *restrict, uid_t *restrict)))
		pc_fail("getresuid() is not declared as int getresuid(uid_t *restrict, uid_t *restrict, uid_t *restrict)");
	if (IS_T(&(getresuid), int (*)(uid_t *restrict, uid_t *restrict, uid_t *restrict, int)))
		pc_fail("getresuid() is declared without a prototype");
	if (!IS_T(&(setresgid), int (*)(gid_t, gid_t, gid_t)))
		pc_fail("setresgid() is not declared as int setresgid(gid_t, gid_t, gid_t)");
	if (IS_T(&(setresgid), int (*)(gid_t, gid_t, gid_t, int)))
		pc_fail("setresgid() is declared without a prototype");
	if (!IS_T(&(setresuid), int (*)(uid_t, uid_t, uid_t)))
		pc_fail("setresuid() is not declared as int setresuid(uid_t, uid_t, uid_t)");
	if (IS_T(&(setresuid), int (*)(uid_t, uid_t, uid_t, int)))
		pc_fail("setresuid() is declared without a prototype");
	pc_pass();
}
