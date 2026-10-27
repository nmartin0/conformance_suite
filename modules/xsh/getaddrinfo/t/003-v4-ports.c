/*
 * Numeric port strings at the ends of the range and a middle value are encoded exactly.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getaddrinfo.003
 */
#include "ga.h"

int main(void)
{
	static const char *const strs[] = { "0", "80", "65535" };
	static const unsigned want[] = { 0, 80, 65535 };
	size_t i;

	pc_begin("XSH.getaddrinfo.003");
	pc_test();
	for (i = 0; i < 3; i++) {
		struct addrinfo *res = gai_ok("127.0.0.1", strs[i], NUMH, AF_INET, SOCK_DGRAM, 0);
		const struct sockaddr_in *s = sin_of(res);

		pc_check(ntohs(s->sin_port) == want[i], "service \"%s\" gave port %u", strs[i], (unsigned)ntohs(s->sin_port));
		freeaddrinfo(res);
	}
	pc_pass();
}
