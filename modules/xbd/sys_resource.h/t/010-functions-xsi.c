/*
 * <sys/resource.h> functions xsi
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sys_resource.h.010
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
	pc_begin("XBD.sys_resource.h.010");
	pc_test();
	if (!IS_T(&(getpriority), int (*)(int, id_t)))
		pc_fail("getpriority() is not declared as int getpriority(int, id_t)");
	if (IS_T(&(getpriority), int (*)(int, id_t, int)))
		pc_fail("getpriority() is declared without a prototype");
	if (!IS_T(&(getrusage), int (*)(int, struct rusage *)))
		pc_fail("getrusage() is not declared as int getrusage(int, struct rusage *)");
	if (IS_T(&(getrusage), int (*)(int, struct rusage *, int)))
		pc_fail("getrusage() is declared without a prototype");
	if (!IS_T(&(setpriority), int (*)(int, id_t, int)))
		pc_fail("setpriority() is not declared as int setpriority(int, id_t, int)");
	if (IS_T(&(setpriority), int (*)(int, id_t, int, int)))
		pc_fail("setpriority() is declared without a prototype");
	pc_pass();
}
