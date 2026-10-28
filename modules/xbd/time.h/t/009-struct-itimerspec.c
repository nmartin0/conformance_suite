/*
 * <time.h> struct itimerspec
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.time.h.009
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

typedef char pc_c0[sizeof(struct itimerspec)];	/* a complete type */
static struct itimerspec *ps;

int main(void)
{
	pc_begin("XBD.time.h.009");
	pc_test();
	(void)ps;
	if (!IS_T(&ps->it_interval, struct timespec *))
		pc_fail("struct itimerspec member it_interval is not of type struct timespec");
	if (!IS_T(&ps->it_value, struct timespec *))
		pc_fail("struct itimerspec member it_value is not of type struct timespec");
	pc_pass();
}
