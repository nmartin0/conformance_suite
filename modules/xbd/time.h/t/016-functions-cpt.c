/*
 * <time.h> functions cpt
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.time.h.016
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
	pc_begin("XBD.time.h.016");
	pc_test();
	if (!IS_T(&(clock_getcpuclockid), int (*)(pid_t, clockid_t *)))
		pc_fail("clock_getcpuclockid() is not declared as int clock_getcpuclockid(pid_t, clockid_t *)");
	if (IS_T(&(clock_getcpuclockid), int (*)(pid_t, clockid_t *, int)))
		pc_fail("clock_getcpuclockid() is declared without a prototype");
	pc_pass();
}
