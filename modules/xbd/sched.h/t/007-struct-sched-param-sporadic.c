/*
 * <sched.h> struct sched param sporadic
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sched.h.007
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

typedef char pc_c0[sizeof(struct sched_param)];	/* a complete type */
static struct sched_param *ps;

int main(void)
{
	pc_begin("XBD.sched.h.007");
	pc_test();
	(void)ps;
	if (!IS_T(&ps->sched_ss_low_priority, int *))
		pc_fail("struct sched_param member sched_ss_low_priority is not of type int");
	if (!IS_T(&ps->sched_ss_repl_period, struct timespec *))
		pc_fail("struct sched_param member sched_ss_repl_period is not of type struct timespec");
	if (!IS_T(&ps->sched_ss_init_budget, struct timespec *))
		pc_fail("struct sched_param member sched_ss_init_budget is not of type struct timespec");
	if (!IS_T(&ps->sched_ss_max_repl, int *))
		pc_fail("struct sched_param member sched_ss_max_repl is not of type int");
	pc_pass();
}
