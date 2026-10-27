/*
 * A numeric IPv4 host and port give stream entries with the right family, port, address.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getaddrinfo.001
 */
#include "ga.h"

int main(void)
{
	struct addrinfo *res, *p;

	pc_begin("XSH.getaddrinfo.001");
	pc_test();
	res = gai_ok("127.0.0.1", "8080", NUMH, AF_INET, SOCK_STREAM, 0);
	for (p = res; p != NULL; p = p->ai_next) {
		const struct sockaddr_in *s = sin_of(p);
		unsigned char port[2], addr[4];

		pc_check(p->ai_socktype == SOCK_STREAM, "entry type %d, expected SOCK_STREAM", p->ai_socktype);
		v4_bytes(s, port, addr);
		pc_check(ntohs(s->sin_port) == 8080, "port is %u, expected 8080", (unsigned)ntohs(s->sin_port));
		pc_check(addr[0] == 127 && addr[1] == 0 && addr[2] == 0 && addr[3] == 1, "address is not 127.0.0.1");
		socketable(p, -1);
	}
	freeaddrinfo(res);
	pc_pass();
}
