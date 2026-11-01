/*
 * <pwd.h> functions
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.pwd.h.003
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
	pc_begin("XBD.pwd.h.003");
	pc_test();
	if (!IS_T(&(getpwnam), struct passwd *(*)(const char *)))
		pc_fail("getpwnam() is not declared with the prototype the page gives");
	if (!IS_T(&(getpwnam_r), int (*)(const char *, struct passwd *, char *, size_t, struct passwd **)))
		pc_fail("getpwnam_r() is not declared with the prototype the page gives");
	if (!IS_T(&(getpwuid), struct passwd *(*)(uid_t)))
		pc_fail("getpwuid() is not declared with the prototype the page gives");
	if (!IS_T(&(getpwuid_r), int (*)(uid_t, struct passwd *, char *, size_t, struct passwd **)))
		pc_fail("getpwuid_r() is not declared with the prototype the page gives");
	pc_pass();
}
