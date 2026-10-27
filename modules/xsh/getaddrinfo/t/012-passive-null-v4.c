/*
 * AI_PASSIVE with no node gives the IPv4 wildcard address and the requested port.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getaddrinfo.012
 */
#include "ga.h"

int main(void)
{
	struct addrinfo *res, *p;

	pc_begin("XSH.getaddrinfo.012");
	pc_test();
	res = gai_ok(NULL, "9", AI_PASSIVE | AI_NUMERICSERV, AF_INET, SOCK_STREAM, 0);
	for (p = res; p != NULL; p = p->ai_next) {
		const struct sockaddr_in *s = sin_of(p);

		pc_check(is_wild4(s), "the address is not INADDR_ANY");
		pc_check(ntohs(s->sin_port) == 9, "port is %u, expected 9", (unsigned)ntohs(s->sin_port));
	}
	freeaddrinfo(res);
	pc_pass();
}
