/*
 * Without AI_PASSIVE and with no node, an AF_INET6 entry holds the IPv6 loopback address.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getaddrinfo.017
 */
#include "ga.h"

int main(void)
{
	struct addrinfo *res, *p;

	pc_begin("XSH.getaddrinfo.017");
	need_v6();
	pc_test();
	res = gai_ok(NULL, "9", AI_NUMERICSERV, AF_INET6, SOCK_STREAM, 0);
	for (p = res; p != NULL; p = p->ai_next)
		pc_check(IN6_IS_ADDR_LOOPBACK(&sin6_of(p)->sin6_addr), "the address is not the loopback address");
	freeaddrinfo(res);
	pc_pass();
}
