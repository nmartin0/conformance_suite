/*
 * AI_V4MAPPED and AI_ALL are ignored when the family is AF_INET.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getaddrinfo.011
 */
#include "ga.h"

int main(void)
{
	static const int fl[] = { AI_V4MAPPED, AI_ALL, AI_V4MAPPED | AI_ALL };
	size_t i;

	pc_begin("XSH.getaddrinfo.011");
	pc_test();
	for (i = 0; i < 3; i++) {
		struct addrinfo *res = gai_ok("127.0.0.1", "80", NUMH | fl[i], AF_INET, SOCK_STREAM, 0), *p;

		for (p = res; p != NULL; p = p->ai_next) {
			const struct sockaddr_in *s = sin_of(p);
			unsigned char port[2], addr[4];

			v4_bytes(s, port, addr);
			pc_check(addr[0] == 127 && addr[3] == 1, "address is not 127.0.0.1 (flags %#x)", (unsigned)fl[i]);
		}
		freeaddrinfo(res);
	}
	pc_pass();
}
