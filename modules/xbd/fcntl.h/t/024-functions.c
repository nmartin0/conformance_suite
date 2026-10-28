/*
 * <fcntl.h> functions
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.fcntl.h.024
 *
 * Only <fcntl.h> is included (not pc.h, which brings in <stddef.h>), so a name
 * the header fails to provide stops the build, which the module reports as FAIL.
 */
#include <fcntl.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

int main(void)
{
	pc_begin("XBD.fcntl.h.024");
	pc_test();
	if (!IS_T(&(creat), int (*)(const char *, mode_t)))
		pc_fail("creat() is not declared as int creat(const char *, mode_t)");
	if (IS_T(&(creat), int (*)(const char *, mode_t, int)))
		pc_fail("creat() is declared without a prototype");
	if (!IS_T(&(fcntl), int (*)(int, int, ...)))
		pc_fail("fcntl() is not declared as int fcntl(int, int, ...)");
	if (!IS_T(&(open), int (*)(const char *, int, ...)))
		pc_fail("open() is not declared as int open(const char *, int, ...)");
	if (!IS_T(&(openat), int (*)(int, const char *, int, ...)))
		pc_fail("openat() is not declared as int openat(int, const char *, int, ...)");
	pc_pass();
}
