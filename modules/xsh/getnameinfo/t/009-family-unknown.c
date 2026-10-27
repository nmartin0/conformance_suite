/*
 * A socket address whose family is not recognized fails with EAI_FAMILY.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getnameinfo.009
 */
#include "ni.h"

int main(void)
{
	struct sockaddr_storage ss;
	sa_family_t bad = (sa_family_t)-1;
	char node[64], serv[32];
	int rc;

	pc_begin("XSH.getnameinfo.009");
	if (bad == AF_INET || bad == AF_INET6 || bad == AF_UNIX || bad == AF_UNSPEC)
		pc_unresolved("the largest family value is a known family here");
	memset(&ss, 0, sizeof ss);
	ss.ss_family = bad;
	pc_test();
	rc = getnameinfo((struct sockaddr *)&ss, sizeof ss, node, sizeof node, serv, sizeof serv, NUMF);
	pc_check(rc == EAI_FAMILY, "an unknown family gave %d (%s), expected EAI_FAMILY", rc, gai_strerror(rc));
	pc_pass();
}
