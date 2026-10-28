/*
 * <sys/utsname.h> functions
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sys_utsname.h.002
 *
 * Only <sys/utsname.h> is included (not pc.h, which brings in <stddef.h>), so a name
 * the header fails to provide stops the build, which the module reports as FAIL.
 */
#include <sys/utsname.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

int main(void)
{
	pc_begin("XBD.sys_utsname.h.002");
	pc_test();
	if (!IS_T(&(uname), int (*)(struct utsname *)))
		pc_fail("uname() is not declared as int uname(struct utsname *)");
	if (IS_T(&(uname), int (*)(struct utsname *, int)))
		pc_fail("uname() is declared without a prototype");
	pc_pass();
}
