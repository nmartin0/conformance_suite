/*
 * <sys/socket.h> struct linger
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sys_socket.h.021
 *
 * Only <sys/socket.h> is included (not pc.h, which brings in <stddef.h>), so a name
 * the header fails to provide stops the build, which the module reports as FAIL.
 */
#include <sys/socket.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

typedef char pc_c0[sizeof(struct linger)];	/* a complete type */
static struct linger *ps;

int main(void)
{
	pc_begin("XBD.sys_socket.h.021");
	pc_test();
	(void)ps;
	if (!IS_T(&ps->l_onoff, int *))
		pc_fail("struct linger member l_onoff is not of type int");
	if (!IS_T(&ps->l_linger, int *))
		pc_fail("struct linger member l_linger is not of type int");
	pc_pass();
}
