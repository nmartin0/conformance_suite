/*
 * With AF_UNSPEC an IPv4 address string gives only AF_INET entries.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getaddrinfo.005
 */
#include "ga.h"

int main(void)
{
	struct addrinfo *res, *p;

	pc_begin("XSH.getaddrinfo.005");
	pc_test();
	res = gai_ok("127.0.0.1", "80", NUMH, AF_UNSPEC, 0, 0);
	for (p = res; p != NULL; p = p->ai_next) {
		const struct sockaddr_in *s = sin_of(p);

		pc_check(ntohs(s->sin_port) == 80, "port is %u", (unsigned)ntohs(s->sin_port));
	}
	freeaddrinfo(res);
	pc_pass();
}
