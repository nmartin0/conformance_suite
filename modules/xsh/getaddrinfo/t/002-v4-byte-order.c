/*
 * IPv4 address and port are stored in network byte order.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getaddrinfo.002
 */
#include "ga.h"

int main(void)
{
	struct addrinfo *res;
	const struct sockaddr_in *s;
	unsigned char port[2], addr[4];

	pc_begin("XSH.getaddrinfo.002");
	pc_test();
	res = gai_ok("10.20.30.40", "258", NUMH, AF_INET, SOCK_STREAM, 0);
	s = sin_of(res);
	v4_bytes(s, port, addr);
	pc_check(addr[0] == 10 && addr[1] == 20 && addr[2] == 30 && addr[3] == 40,
	    "address bytes are %u.%u.%u.%u, expected 10.20.30.40", addr[0], addr[1], addr[2], addr[3]);
	pc_check(port[0] == 1 && port[1] == 2, "port bytes are %u %u, expected 1 2 (258 in network order)", port[0], port[1]);
	freeaddrinfo(res);
	pc_pass();
}
