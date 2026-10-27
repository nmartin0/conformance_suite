/*
 * AI_V4MAPPED with AF_INET6 turns an IPv4 address string into an IPv4-mapped address.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getaddrinfo.010
 */
#include "ga.h"

int main(void)
{
	struct addrinfo *res, *p;

	pc_begin("XSH.getaddrinfo.010");
	need_v6();
	pc_test();
	res = gai_ok("127.0.0.1", "80", NUMH | AI_V4MAPPED, AF_INET6, SOCK_STREAM, 0);
	for (p = res; p != NULL; p = p->ai_next) {
		const struct sockaddr_in6 *s = sin6_of(p);
		const unsigned char *b = s->sin6_addr.s6_addr;

		pc_check(IN6_IS_ADDR_V4MAPPED(&s->sin6_addr), "address is not IPv4-mapped");
		pc_check(b[12] == 127 && b[13] == 0 && b[14] == 0 && b[15] == 1, "embedded address is not 127.0.0.1");
	}
	freeaddrinfo(res);
	pc_pass();
}
