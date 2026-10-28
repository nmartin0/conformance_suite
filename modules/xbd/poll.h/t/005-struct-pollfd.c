/*
 * <poll.h> struct pollfd
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.poll.h.005
 *
 * Only <poll.h> is included (not pc.h, which brings in <stddef.h>), so a name
 * the header fails to provide stops the build, which the module reports as FAIL.
 */
#include <poll.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

typedef char pc_c0[sizeof(struct pollfd)];	/* a complete type */
static struct pollfd *ps;

int main(void)
{
	pc_begin("XBD.poll.h.005");
	pc_test();
	(void)ps;
	if (!IS_T(&ps->fd, int *))
		pc_fail("struct pollfd member fd is not of type int");
	if (!IS_T(&ps->events, short *))
		pc_fail("struct pollfd member events is not of type short");
	if (!IS_T(&ps->revents, short *))
		pc_fail("struct pollfd member revents is not of type short");
	pc_pass();
}
