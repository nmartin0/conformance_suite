/*
 * <sys/time.h> struct timeval and fd_set
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sys_time.h.001
 *
 * Only <sys/time.h> is included (not pc.h, which brings in other headers), so a
 * name the header fails to provide stops the build, which the module
 * reports as FAIL.
 */
#include <sys/time.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

typedef char pc_c0[sizeof(struct timeval)];	/* a complete type */
static struct timeval *ps;
typedef fd_set *pc_t0;	/* the type exists */
typedef char pc_c1[sizeof(fd_set)];	/* and is complete */

int main(void)
{
	pc_begin("XBD.sys_time.h.001");
	pc_test();
	(void)ps;
	if (!IS_T(&ps->tv_sec, time_t *))
		pc_fail("struct timeval member tv_sec is not of type time_t");
	if (!IS_T(&ps->tv_usec, suseconds_t *))
		pc_fail("struct timeval member tv_usec is not of type suseconds_t");
	pc_pass();
}
