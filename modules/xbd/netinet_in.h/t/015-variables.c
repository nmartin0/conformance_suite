/*
 * <netinet/in.h> variables
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.netinet_in.h.015
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

int main(void)
{
	pc_begin("XBD.netinet_in.h.015");
	pc_test();
	if (!IS_T(&in6addr_any, const struct in6_addr *))
		pc_fail("in6addr_any is not declared as const struct in6_addr");
	if (!IS_T(&in6addr_loopback, const struct in6_addr *))
		pc_fail("in6addr_loopback is not declared as const struct in6_addr");
	pc_pass();
}
