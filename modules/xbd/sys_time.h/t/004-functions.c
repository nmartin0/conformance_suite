/*
 * <sys/time.h> functions
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sys_time.h.004
 *
 * Only <sys/time.h> is included (not pc.h, which brings in other headers), so a
 * name the header fails to provide stops the build, which the module
 * reports as FAIL.
 */
#include <sys/time.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

int main(void)
{
	pc_begin("XBD.sys_time.h.004");
	pc_test();
	if (!IS_T(&(select), int (*)(int, fd_set *, fd_set *, fd_set *, struct timeval *)))
		pc_fail("select() is not declared with the prototype the page gives");
	if (!IS_T(&(utimes), int (*)(const char *, const struct timeval *)))
		pc_fail("utimes() is not declared with the prototype the page gives");
	pc_pass();
}
