/*
 * <sys/socket.h> struct msghdr
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sys_socket.h.018
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

typedef char pc_c0[sizeof(struct msghdr)];	/* a complete type */
static struct msghdr *ps;

int main(void)
{
	pc_begin("XBD.sys_socket.h.018");
	pc_test();
	(void)ps;
	if (!IS_T(&ps->msg_name, void * *))
		pc_fail("struct msghdr member msg_name is not of type void *");
	if (!IS_T(&ps->msg_namelen, socklen_t *))
		pc_fail("struct msghdr member msg_namelen is not of type socklen_t");
	if (!IS_T(&ps->msg_iov, struct iovec * *))
		pc_fail("struct msghdr member msg_iov is not of type struct iovec *");
	if (!IS_T(&ps->msg_iovlen, int *))
		pc_fail("struct msghdr member msg_iovlen is not of type int");
	if (!IS_T(&ps->msg_control, void * *))
		pc_fail("struct msghdr member msg_control is not of type void *");
	if (!IS_T(&ps->msg_controllen, socklen_t *))
		pc_fail("struct msghdr member msg_controllen is not of type socklen_t");
	if (!IS_T(&ps->msg_flags, int *))
		pc_fail("struct msghdr member msg_flags is not of type int");
	pc_pass();
}
