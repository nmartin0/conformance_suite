/*
 * <sys/socket.h> struct cmsghdr
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sys_socket.h.020
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

typedef char pc_c0[sizeof(struct cmsghdr)];	/* a complete type */
static struct cmsghdr *ps;

int main(void)
{
	pc_begin("XBD.sys_socket.h.020");
	pc_test();
	(void)ps;
	if (!IS_T(&ps->cmsg_len, socklen_t *))
		pc_fail("struct cmsghdr member cmsg_len is not of type socklen_t");
	if (!IS_T(&ps->cmsg_level, int *))
		pc_fail("struct cmsghdr member cmsg_level is not of type int");
	if (!IS_T(&ps->cmsg_type, int *))
		pc_fail("struct cmsghdr member cmsg_type is not of type int");
	pc_pass();
}
