/*
 * <sys/uio.h> functions
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sys_uio.h.003
 *
 * Only <sys/uio.h> is included (not pc.h, which brings in other headers), so a
 * name the header fails to provide stops the build, which the module
 * reports as FAIL.
 */
#include <sys/uio.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

int main(void)
{
	pc_begin("XBD.sys_uio.h.003");
	pc_test();
	if (!IS_T(&(readv), ssize_t (*)(int, const struct iovec *, int)))
		pc_fail("readv() is not declared with the prototype the page gives");
	if (!IS_T(&(writev), ssize_t (*)(int, const struct iovec *, int)))
		pc_fail("writev() is not declared with the prototype the page gives");
	pc_pass();
}
