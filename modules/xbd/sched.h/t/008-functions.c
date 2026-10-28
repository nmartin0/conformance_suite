/*
 * <sched.h> functions
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sched.h.008
 *
 * Only <sched.h> is included (not pc.h, which brings in <stddef.h>), so a name
 * the header fails to provide stops the build, which the module reports as FAIL.
 */
#include <sched.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

int main(void)
{
	pc_begin("XBD.sched.h.008");
	pc_test();
	if (!IS_T(&(sched_yield), int (*)(void)))
		pc_fail("sched_yield() is not declared as int sched_yield(void)");
	if (IS_T(&(sched_yield), int (*)(int)))
		pc_fail("sched_yield() is declared without a prototype");
	pc_pass();
}
