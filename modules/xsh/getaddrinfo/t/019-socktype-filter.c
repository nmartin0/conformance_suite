/*
 * A non-zero socket type in the hints limits every entry to that type.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getaddrinfo.019
 */
#include "ga.h"

int main(void)
{
	static const int types[] = { SOCK_STREAM, SOCK_DGRAM };
	size_t i;

	pc_begin("XSH.getaddrinfo.019");
	pc_test();
	for (i = 0; i < 2; i++) {
		struct addrinfo *res = gai_ok("127.0.0.1", "7", NUMH, AF_INET, types[i], 0), *p;

		for (p = res; p != NULL; p = p->ai_next) {
			pc_check(p->ai_socktype == types[i], "entry type %d, hint %d", p->ai_socktype, types[i]);
			socketable(p, -1);
		}
		freeaddrinfo(res);
	}
	pc_pass();
}
