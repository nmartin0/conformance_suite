/*
 * The IPv6 unspecified address with NI_NAMEREQD fails with EAI_NONAME, as its name cannot be located.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getnameinfo.013
 */
#include "ni.h"

int main(void)
{
	unsigned char a[16];
	struct sockaddr_in6 s;
	char node[128], serv[32];
	int rc;

	pc_begin("XSH.getnameinfo.013");
	need_v6();
	memset(a, 0, sizeof a);
	s = mk6(a, 80);
	pc_test();
	rc = getnameinfo((struct sockaddr *)&s, sizeof s, node, sizeof node, serv, sizeof serv, NI_NAMEREQD | NI_NUMERICSERV);
	pc_check(rc == EAI_NONAME, "NI_NAMEREQD with :: gave %d (%s), expected EAI_NONAME", rc, gai_strerror(rc));
	pc_pass();
}
