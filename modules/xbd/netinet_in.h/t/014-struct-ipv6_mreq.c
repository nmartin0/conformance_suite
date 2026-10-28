/*
 * <netinet/in.h> struct ipv6_mreq
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.netinet_in.h.014
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

typedef char pc_c0[sizeof(struct ipv6_mreq)];	/* a complete type */
static struct ipv6_mreq *ps;

int main(void)
{
	pc_begin("XBD.netinet_in.h.014");
	pc_test();
	(void)ps;
	if (!IS_T(&ps->ipv6mr_multiaddr, struct in6_addr *))
		pc_fail("struct ipv6_mreq member ipv6mr_multiaddr is not of type struct in6_addr");
	if (!IS_T(&ps->ipv6mr_interface, unsigned *))
		pc_fail("struct ipv6_mreq member ipv6mr_interface is not of type unsigned");
	pc_pass();
}
