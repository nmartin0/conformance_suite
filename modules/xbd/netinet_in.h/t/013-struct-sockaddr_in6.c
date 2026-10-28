/*
 * <netinet/in.h> struct sockaddr_in6
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.netinet_in.h.013
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

typedef char pc_c0[sizeof(struct sockaddr_in6)];	/* a complete type */
static struct sockaddr_in6 *ps;

int main(void)
{
	pc_begin("XBD.netinet_in.h.013");
	pc_test();
	(void)ps;
	if (!IS_T(&ps->sin6_family, sa_family_t *))
		pc_fail("struct sockaddr_in6 member sin6_family is not of type sa_family_t");
	if (!IS_T(&ps->sin6_port, in_port_t *))
		pc_fail("struct sockaddr_in6 member sin6_port is not of type in_port_t");
	if (!IS_T(&ps->sin6_flowinfo, uint32_t *))
		pc_fail("struct sockaddr_in6 member sin6_flowinfo is not of type uint32_t");
	if (!IS_T(&ps->sin6_addr, struct in6_addr *))
		pc_fail("struct sockaddr_in6 member sin6_addr is not of type struct in6_addr");
	if (!IS_T(&ps->sin6_scope_id, uint32_t *))
		pc_fail("struct sockaddr_in6 member sin6_scope_id is not of type uint32_t");
	pc_pass();
}
