/*
 * Numeric host and service of an IPv6 address come back as an address text for it and the decimal port.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getnameinfo.002
 */
#include "ni.h"

int main(void)
{
	static const unsigned char a1[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 };
	static const unsigned char a2[16] = { 0x20, 0x01, 0x0d, 0xb8, 0, 0, 0, 0, 0, 0, 0, 0, 0xff, 0, 0x42, 0x83 };
	const unsigned char *addrs[2] = { a1, a2 };
	static const unsigned ports[] = { 443, 65535 };
	size_t i;

	pc_begin("XSH.getnameinfo.002");
	need_v6();
	pc_test();
	for (i = 0; i < 2; i++) {
		struct sockaddr_in6 s = mk6(addrs[i], ports[i]);
		char node[128], serv[32];
		unsigned char got[16];
		int rc, ok;

		memset(node, 0xAA, sizeof node);
		memset(serv, 0xAA, sizeof serv);
		rc = getnameinfo((struct sockaddr *)&s, sizeof s, node, sizeof node, serv, sizeof serv, NUMF);
		pc_check(rc == 0, "getnameinfo failed: %s", gai_strerror(rc));
		pc_check(terminated(node, sizeof node) && terminated(serv, sizeof serv), "a returned name is not null-terminated");
		pc_check(inet_pton(AF_INET6, node, got) == 1, "node \"%s\" is not an IPv6 address string", node);
		pc_check(memcmp(got, addrs[i], 16) == 0, "node \"%s\" is another address", node);
		pc_check(digits_value(serv, &ok) == ports[i] && ok, "service \"%s\" is not the decimal port %u", serv, ports[i]);
	}
	pc_pass();
}
