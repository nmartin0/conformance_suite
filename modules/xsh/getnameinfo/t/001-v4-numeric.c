/*
 * Numeric host and service of an IPv4 address come back as the address text and the decimal port.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getnameinfo.001
 */
#include "ni.h"

int main(void)
{
	static const unsigned char addrs[][4] = { { 127, 0, 0, 1 }, { 10, 20, 30, 40 }, { 192, 168, 100, 200 }, { 0, 0, 0, 0 } };
	static const unsigned ports[] = { 8080, 258, 65535, 0 };
	size_t i;

	pc_begin("XSH.getnameinfo.001");
	pc_test();
	for (i = 0; i < 4; i++) {
		struct sockaddr_in s = mk4(addrs[i], ports[i]);
		char node[64], serv[32];
		unsigned char got[4];
		int rc, ok;

		memset(node, 0xAA, sizeof node);
		memset(serv, 0xAA, sizeof serv);
		rc = getnameinfo((struct sockaddr *)&s, sizeof s, node, sizeof node, serv, sizeof serv, NUMF);
		pc_check(rc == 0, "getnameinfo failed: %s", gai_strerror(rc));
		pc_check(terminated(node, sizeof node) && terminated(serv, sizeof serv), "a returned name is not null-terminated");
		pc_check(inet_pton(AF_INET, node, got) == 1, "node \"%s\" is not an IPv4 address string", node);
		pc_check(memcmp(got, addrs[i], 4) == 0, "node \"%s\" is another address", node);
		pc_check(digits_value(serv, &ok) == ports[i] && ok, "service \"%s\" is not the decimal port %u", serv, ports[i]);
	}
	pc_pass();
}
