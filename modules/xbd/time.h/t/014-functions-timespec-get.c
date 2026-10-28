/*
 * <time.h> functions timespec get
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.time.h.014
 *
 * Only <time.h> is included (not pc.h, which brings in <stddef.h>), so a name
 * the header fails to provide stops the build, which the module reports as FAIL.
 */
#include <time.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

int main(void)
{
	pc_begin("XBD.time.h.014");
	pc_test();
	if (!IS_T(&(timespec_get), int (*)(struct timespec *, int)))
		pc_fail("timespec_get() is not declared as int timespec_get(struct timespec *, int)");
	if (IS_T(&(timespec_get), int (*)(struct timespec *, int, int)))
		pc_fail("timespec_get() is declared without a prototype");
	pc_pass();
}
