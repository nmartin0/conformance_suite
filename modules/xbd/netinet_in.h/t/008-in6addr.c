/*
 * in6addr_any is ::, in6addr_loopback is ::1, and the two INIT macros
 *   are static initializers for them.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.netinet_in.h.008
 */
#include <netinet/in.h>
#include <string.h>
#include "pc.h"

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

static const struct in6_addr init_any = IN6ADDR_ANY_INIT;
static const struct in6_addr init_loop = IN6ADDR_LOOPBACK_INIT;

int main(void)
{
	unsigned char zero[16], one[16];
	int i;

	memset(zero, 0, sizeof zero);
	memset(one, 0, sizeof one);
	one[15] = 1;
	pc_begin("XBD.netinet_in.h.008");
	pc_test();
	pc_check(IS_T(&in6addr_any, const struct in6_addr *), "in6addr_any is not a const struct in6_addr");
	pc_check(IS_T(&in6addr_loopback, const struct in6_addr *), "in6addr_loopback is not a const struct in6_addr");
	for (i = 0; i < 16; i++) {
		pc_check(in6addr_any.s6_addr[i] == zero[i], "in6addr_any is not ::");
		pc_check(in6addr_loopback.s6_addr[i] == one[i], "in6addr_loopback is not ::1");
		pc_check(init_any.s6_addr[i] == zero[i], "IN6ADDR_ANY_INIT is not ::");
		pc_check(init_loop.s6_addr[i] == one[i], "IN6ADDR_LOOPBACK_INIT is not ::1");
	}
	pc_check(IN6_IS_ADDR_UNSPECIFIED(&in6addr_any), "IN6_IS_ADDR_UNSPECIFIED rejects ::");
	pc_check(!IN6_IS_ADDR_UNSPECIFIED(&in6addr_loopback), "IN6_IS_ADDR_UNSPECIFIED accepts ::1");
	pc_check(IN6_IS_ADDR_LOOPBACK(&in6addr_loopback), "IN6_IS_ADDR_LOOPBACK rejects ::1");
	pc_check(!IN6_IS_ADDR_LOOPBACK(&in6addr_any), "IN6_IS_ADDR_LOOPBACK accepts ::");
	pc_pass();
}
