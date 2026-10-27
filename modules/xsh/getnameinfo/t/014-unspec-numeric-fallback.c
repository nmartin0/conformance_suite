/*
 * The IPv6 unspecified address without a host flag yields its numeric form, since no lookup is made.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getnameinfo.014
 */
#include "ni.h"

int main(void)
{
	unsigned char a[16], got[16];
	struct sockaddr_in6 s;
	char node[128], serv[32];
	int rc;

	pc_begin("XSH.getnameinfo.014");
	need_v6();
	memset(a, 0, sizeof a);
	s = mk6(a, 80);
	pc_test();
	memset(node, 0xAA, sizeof node);
	rc = getnameinfo((struct sockaddr *)&s, sizeof s, node, sizeof node, serv, sizeof serv, NI_NUMERICSERV);
	pc_check(rc == 0, "getnameinfo for :: gave %d (%s)", rc, gai_strerror(rc));
	pc_check(terminated(node, sizeof node) && inet_pton(AF_INET6, node, got) == 1, "node \"%s\" is not an IPv6 address string", node);
	pc_check(memcmp(got, a, 16) == 0, "node \"%s\" is not the unspecified address", node);
	pc_pass();
}
