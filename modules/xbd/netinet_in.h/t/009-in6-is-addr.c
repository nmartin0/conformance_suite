/*
 * The IN6_IS_ADDR_ test macros take a const struct in6_addr * and have
 *   type int.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.netinet_in.h.009
 */
#include <netinet/in.h>
#include "pc.h"

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

int main(void)
{
	const struct in6_addr *p = &in6addr_any;

	pc_begin("XBD.netinet_in.h.009");
	pc_test();
	pc_check(IS_T(IN6_IS_ADDR_UNSPECIFIED(p), int), "IN6_IS_ADDR_UNSPECIFIED() does not have type int");
	pc_check(IS_T(IN6_IS_ADDR_LOOPBACK(p), int), "IN6_IS_ADDR_LOOPBACK() does not have type int");
	pc_check(IS_T(IN6_IS_ADDR_MULTICAST(p), int), "IN6_IS_ADDR_MULTICAST() does not have type int");
	pc_check(IS_T(IN6_IS_ADDR_LINKLOCAL(p), int), "IN6_IS_ADDR_LINKLOCAL() does not have type int");
	pc_check(IS_T(IN6_IS_ADDR_SITELOCAL(p), int), "IN6_IS_ADDR_SITELOCAL() does not have type int");
	pc_check(IS_T(IN6_IS_ADDR_V4MAPPED(p), int), "IN6_IS_ADDR_V4MAPPED() does not have type int");
	pc_check(IS_T(IN6_IS_ADDR_V4COMPAT(p), int), "IN6_IS_ADDR_V4COMPAT() does not have type int");
	pc_check(IS_T(IN6_IS_ADDR_MC_NODELOCAL(p), int), "IN6_IS_ADDR_MC_NODELOCAL() does not have type int");
	pc_check(IS_T(IN6_IS_ADDR_MC_LINKLOCAL(p), int), "IN6_IS_ADDR_MC_LINKLOCAL() does not have type int");
	pc_check(IS_T(IN6_IS_ADDR_MC_SITELOCAL(p), int), "IN6_IS_ADDR_MC_SITELOCAL() does not have type int");
	pc_check(IS_T(IN6_IS_ADDR_MC_ORGLOCAL(p), int), "IN6_IS_ADDR_MC_ORGLOCAL() does not have type int");
	pc_check(IS_T(IN6_IS_ADDR_MC_GLOBAL(p), int), "IN6_IS_ADDR_MC_GLOBAL() does not have type int");
	pc_pass();
}
