/*
 * With AF_UNSPEC an IPv6 address string gives only AF_INET6 entries holding that address.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getaddrinfo.008
 */
#include "ga.h"

int main(void)
{
	struct addrinfo *res, *p;

	pc_begin("XSH.getaddrinfo.008");
	need_v6();
	pc_test();
	res = gai_ok("::1", "80", NUMH, AF_UNSPEC, 0, 0);
	for (p = res; p != NULL; p = p->ai_next) {
		const struct sockaddr_in6 *s = sin6_of(p);

		pc_check(IN6_IS_ADDR_LOOPBACK(&s->sin6_addr), "address is not ::1");
		pc_check(ntohs(s->sin6_port) == 80, "port is %u", (unsigned)ntohs(s->sin6_port));
	}
	freeaddrinfo(res);
	pc_pass();
}
