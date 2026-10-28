/*
 * <sched.h> functions ps
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sched.h.009
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
	pc_begin("XBD.sched.h.009");
	pc_test();
	if (!IS_T(&(sched_getparam), int (*)(pid_t, struct sched_param *)))
		pc_fail("sched_getparam() is not declared as int sched_getparam(pid_t, struct sched_param *)");
	if (IS_T(&(sched_getparam), int (*)(pid_t, struct sched_param *, int)))
		pc_fail("sched_getparam() is declared without a prototype");
	if (!IS_T(&(sched_getscheduler), int (*)(pid_t)))
		pc_fail("sched_getscheduler() is not declared as int sched_getscheduler(pid_t)");
	if (IS_T(&(sched_getscheduler), int (*)(pid_t, int)))
		pc_fail("sched_getscheduler() is declared without a prototype");
	if (!IS_T(&(sched_setparam), int (*)(pid_t, const struct sched_param *)))
		pc_fail("sched_setparam() is not declared as int sched_setparam(pid_t, const struct sched_param *)");
	if (IS_T(&(sched_setparam), int (*)(pid_t, const struct sched_param *, int)))
		pc_fail("sched_setparam() is declared without a prototype");
	if (!IS_T(&(sched_setscheduler), int (*)(pid_t, int, const struct sched_param *)))
		pc_fail("sched_setscheduler() is not declared as int sched_setscheduler(pid_t, int, const struct sched_param *)");
	if (IS_T(&(sched_setscheduler), int (*)(pid_t, int, const struct sched_param *, int)))
		pc_fail("sched_setscheduler() is declared without a prototype");
	pc_pass();
}
