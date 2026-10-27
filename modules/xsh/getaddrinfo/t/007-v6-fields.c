/*
 * A numeric IPv6 host and port give the right sockaddr_in6 contents, with unnamed fields zero.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getaddrinfo.007
 */
#include "ga.h"

int main(void)
{
	static const unsigned char want[16] = { 0x20, 0x01, 0x0d, 0xb8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 };
	struct addrinfo *res;
	const struct sockaddr_in6 *s;
	unsigned char port[2];

	pc_begin("XSH.getaddrinfo.007");
	need_v6();
	pc_test();
	res = gai_ok("2001:db8::1", "443", NUMH, AF_INET6, SOCK_STREAM, 0);
	s = sin6_of(res);
	pc_check(res->ai_socktype == SOCK_STREAM, "entry type %d", res->ai_socktype);
	memcpy(port, &s->sin6_port, 2);
	pc_check(port[0] == 0x01 && port[1] == 0xbb, "port bytes are %02x %02x, expected 01 bb", port[0], port[1]);
	pc_check(memcmp(&s->sin6_addr, want, 16) == 0, "address bytes differ from 2001:db8::1");
	pc_check(s->sin6_flowinfo == 0, "sin6_flowinfo is not zero");
	pc_check(s->sin6_scope_id == 0, "sin6_scope_id is not zero");
	freeaddrinfo(res);
	pc_pass();
}
