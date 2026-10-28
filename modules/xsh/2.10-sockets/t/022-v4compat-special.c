/*
 * The unspecified address and the loopback address are not treated as
 * IPv4-compatible addresses.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.10-sockets.022
 *
 * IN6_IS_ADDR_V4COMPAT() is zero for in6addr_any (::) and in6addr_loopback
 * (::1). As a control, an address that is plainly IPv4-compatible (::10.0.0.7)
 * must give non-zero; if it does not, the test is not in use.
 */
#include "pcsk6.h"

int main(void)
{
	struct in6_addr x;

	pc_begin("XSH.2.10-sockets.022");
	memset(&x, 0, sizeof x);
	x.s6_addr[12] = 10;
	x.s6_addr[15] = 7;
	pc_test();
	if (!IN6_IS_ADDR_V4COMPAT(&x))
		pc_notinuse("IN6_IS_ADDR_V4COMPAT() is zero for ::10.0.0.7, so it cannot tell the special addresses apart");
	pc_check(!IN6_IS_ADDR_V4COMPAT(&in6addr_any), "the unspecified address :: is treated as IPv4-compatible");
	pc_check(!IN6_IS_ADDR_V4COMPAT(&in6addr_loopback), "the loopback address ::1 is treated as IPv4-compatible");
	pc_pass();
}
