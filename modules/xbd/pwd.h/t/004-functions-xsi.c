/*
 * <pwd.h> functions xsi
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.pwd.h.004
 *
 * Only <pwd.h> is included (not pc.h, which brings in other headers), so a
 * name the header fails to provide stops the build, which the module
 * reports as FAIL.
 */
#include <pwd.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

int main(void)
{
	pc_begin("XBD.pwd.h.004");
	pc_test();
	if (!IS_T(&(endpwent), void (*)(void)))
		pc_fail("endpwent() is not declared with the prototype the page gives");
	if (!IS_T(&(getpwent), struct passwd *(*)(void)))
		pc_fail("getpwent() is not declared with the prototype the page gives");
	if (!IS_T(&(setpwent), void (*)(void)))
		pc_fail("setpwent() is not declared with the prototype the page gives");
	pc_pass();
}
