/*
 * <sys/socket.h> struct sockaddr
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sys_socket.h.017
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

typedef char pc_c0[sizeof(struct sockaddr)];	/* a complete type */
static struct sockaddr *ps;

int main(void)
{
	pc_begin("XBD.sys_socket.h.017");
	pc_test();
	(void)ps;
	if (!IS_T(&ps->sa_family, sa_family_t *))
		pc_fail("struct sockaddr member sa_family is not of type sa_family_t");
	if (!IS_T(&ps->sa_data[0], char *))
		pc_fail("struct sockaddr member sa_data is not an array of char");
	if (IS_T(&ps->sa_data, char **))
		pc_fail("struct sockaddr member sa_data is not an array");
	pc_pass();
}
