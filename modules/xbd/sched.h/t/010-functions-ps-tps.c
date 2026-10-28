/*
 * <sched.h> functions ps tps
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sched.h.010
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
	pc_begin("XBD.sched.h.010");
	pc_test();
	if (!IS_T(&(sched_get_priority_max), int (*)(int)))
		pc_fail("sched_get_priority_max() is not declared as int sched_get_priority_max(int)");
	if (IS_T(&(sched_get_priority_max), int (*)(int, int)))
		pc_fail("sched_get_priority_max() is declared without a prototype");
	if (!IS_T(&(sched_get_priority_min), int (*)(int)))
		pc_fail("sched_get_priority_min() is not declared as int sched_get_priority_min(int)");
	if (IS_T(&(sched_get_priority_min), int (*)(int, int)))
		pc_fail("sched_get_priority_min() is declared without a prototype");
	if (!IS_T(&(sched_rr_get_interval), int (*)(pid_t, struct timespec *)))
		pc_fail("sched_rr_get_interval() is not declared as int sched_rr_get_interval(pid_t, struct timespec *)");
	if (IS_T(&(sched_rr_get_interval), int (*)(pid_t, struct timespec *, int)))
		pc_fail("sched_rr_get_interval() is declared without a prototype");
	pc_pass();
}
