/*
 * A non-zero protocol in the hints limits the entries to that protocol.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getaddrinfo.020
 */
#include "ga.h"

int main(void)
{
	static const int protos[] = { IPPROTO_TCP, IPPROTO_UDP };
	size_t i;

	pc_begin("XSH.getaddrinfo.020");
	pc_test();
	for (i = 0; i < 2; i++) {
		struct addrinfo *res = gai_ok("127.0.0.1", "53", NUMH, AF_INET, 0, protos[i]), *p;

		for (p = res; p != NULL; p = p->ai_next) {
			pc_check(p->ai_protocol == protos[i] || p->ai_protocol == 0,
			    "entry protocol %d, hint %d", p->ai_protocol, protos[i]);
			socketable(p, protos[i]);
		}
		freeaddrinfo(res);
	}
	pc_pass();
}
