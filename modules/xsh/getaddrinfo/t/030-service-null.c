/*
 * A null service gives network-level addresses for the node.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getaddrinfo.030
 */
#include "ga.h"

int main(void)
{
	struct addrinfo *res, *p;

	pc_begin("XSH.getaddrinfo.030");
	pc_test();
	res = gai_ok("127.0.0.1", NULL, AI_NUMERICHOST, AF_INET, 0, 0);
	for (p = res; p != NULL; p = p->ai_next) {
		unsigned char port[2], addr[4];

		v4_bytes(sin_of(p), port, addr);
		pc_check(addr[0] == 127 && addr[1] == 0 && addr[2] == 0 && addr[3] == 1, "address is not 127.0.0.1");
		socketable(p, -1);
	}
	freeaddrinfo(res);
	pc_pass();
}
