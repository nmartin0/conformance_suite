/*
 * <netinet/in.h> struct sockaddr_in
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.netinet_in.h.011
 *
 * Only <netinet/in.h> is included (not pc.h, which brings in <stddef.h>), so a name
 * the header fails to provide stops the build, which the module reports as FAIL.
 */
#include <netinet/in.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

typedef char pc_c0[sizeof(struct sockaddr_in)];	/* a complete type */
static struct sockaddr_in *ps;

int main(void)
{
	pc_begin("XBD.netinet_in.h.011");
	pc_test();
	(void)ps;
	if (!IS_T(&ps->sin_family, sa_family_t *))
		pc_fail("struct sockaddr_in member sin_family is not of type sa_family_t");
	if (!IS_T(&ps->sin_port, in_port_t *))
		pc_fail("struct sockaddr_in member sin_port is not of type in_port_t");
	if (!IS_T(&ps->sin_addr, struct in_addr *))
		pc_fail("struct sockaddr_in member sin_addr is not of type struct in_addr");
	pc_pass();
}
