/*
 * AI_PASSIVE with no node and AF_INET6 gives the IPv6 wildcard address.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getaddrinfo.014
 */
#include "ga.h"

int main(void)
{
	struct addrinfo *res, *p;

	pc_begin("XSH.getaddrinfo.014");
	need_v6();
	pc_test();
	res = gai_ok(NULL, "9", AI_PASSIVE | AI_NUMERICSERV, AF_INET6, SOCK_STREAM, 0);
	for (p = res; p != NULL; p = p->ai_next) {
		const struct sockaddr_in6 *s = sin6_of(p);

		pc_check(IN6_IS_ADDR_UNSPECIFIED(&s->sin6_addr), "the address is not the wildcard address");
		pc_check(ntohs(s->sin6_port) == 9, "port is %u, expected 9", (unsigned)ntohs(s->sin6_port));
	}
	freeaddrinfo(res);
	pc_pass();
}
