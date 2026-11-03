/*
 * <sys/times.h> struct tms
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sys_times.h.001
 *
 * Only <sys/times.h> is included (not pc.h, which brings in other headers), so a
 * name the header fails to provide stops the build, which the module
 * reports as FAIL.
 */
#include <sys/times.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

typedef char pc_c0[sizeof(struct tms)];	/* a complete type */
static struct tms *ps;

int main(void)
{
	pc_begin("XBD.sys_times.h.001");
	pc_test();
	(void)ps;
	if (!IS_T(&ps->tms_utime, clock_t *))
		pc_fail("struct tms member tms_utime is not of type clock_t");
	if (!IS_T(&ps->tms_stime, clock_t *))
		pc_fail("struct tms member tms_stime is not of type clock_t");
	if (!IS_T(&ps->tms_cutime, clock_t *))
		pc_fail("struct tms member tms_cutime is not of type clock_t");
	if (!IS_T(&ps->tms_cstime, clock_t *))
		pc_fail("struct tms member tms_cstime is not of type clock_t");
	pc_pass();
}
