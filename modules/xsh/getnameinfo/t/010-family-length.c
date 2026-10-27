/*
 * A length too short to hold an IPv4 socket address fails with EAI_FAMILY.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getnameinfo.010
 */
#include "ni.h"

int main(void)
{
	static const unsigned char a[4] = { 127, 0, 0, 1 };
	struct sockaddr_in s = mk4(a, 80);
	char node[64], serv[32];
	int rc;

	pc_begin("XSH.getnameinfo.010");
	pc_test();
	rc = getnameinfo((struct sockaddr *)&s, (socklen_t)sizeof(sa_family_t), node, sizeof node, serv, sizeof serv, NUMF);
	pc_check(rc == EAI_FAMILY, "a length holding only the family gave %d (%s), expected EAI_FAMILY", rc, gai_strerror(rc));
	pc_pass();
}
