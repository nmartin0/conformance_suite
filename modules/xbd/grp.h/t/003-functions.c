/*
 * <grp.h> functions
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.grp.h.003
 *
 * Only <grp.h> is included (not pc.h, which brings in other headers), so a
 * name the header fails to provide stops the build, which the module
 * reports as FAIL.
 */
#include <grp.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

int main(void)
{
	pc_begin("XBD.grp.h.003");
	pc_test();
	if (!IS_T(&(getgrgid), struct group *(*)(gid_t)))
		pc_fail("getgrgid() is not declared with the prototype the page gives");
	if (!IS_T(&(getgrgid_r), int (*)(gid_t, struct group *, char *, size_t, struct group **)))
		pc_fail("getgrgid_r() is not declared with the prototype the page gives");
	if (!IS_T(&(getgrnam), struct group *(*)(const char *)))
		pc_fail("getgrnam() is not declared with the prototype the page gives");
	if (!IS_T(&(getgrnam_r), int (*)(const char *, struct group *, char *, size_t, struct group **)))
		pc_fail("getgrnam_r() is not declared with the prototype the page gives");
	pc_pass();
}
