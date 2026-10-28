/*
 * <sys/resource.h> struct rusage
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sys_resource.h.006
 *
 * Only <sys/resource.h> is included (not pc.h, which brings in <stddef.h>), so a name
 * the header fails to provide stops the build, which the module reports as FAIL.
 */
#include <sys/resource.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

typedef char pc_c0[sizeof(struct rusage)];	/* a complete type */
static struct rusage *ps;

int main(void)
{
	pc_begin("XBD.sys_resource.h.006");
	pc_test();
	(void)ps;
	if (!IS_T(&ps->ru_utime, struct timeval *))
		pc_fail("struct rusage member ru_utime is not of type struct timeval");
	if (!IS_T(&ps->ru_stime, struct timeval *))
		pc_fail("struct rusage member ru_stime is not of type struct timeval");
	pc_pass();
}
