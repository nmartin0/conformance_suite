/*
 * <time.h> struct timespec sec
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.time.h.008
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

typedef char pc_c0[sizeof(struct timespec)];	/* a complete type */
static struct timespec *ps;

int main(void)
{
	pc_begin("XBD.time.h.008");
	pc_test();
	(void)ps;
	if (!IS_T(&ps->tv_sec, time_t *))
		pc_fail("struct timespec member tv_sec is not of type time_t");
	pc_pass();
}
