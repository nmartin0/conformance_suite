/*
 * With AI_NUMERICHOST a node that is not a numeric address fails with EAI_NONAME.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getaddrinfo.027
 */
#include "ga.h"

int main(void)
{
	int rc;

	pc_begin("XSH.getaddrinfo.027");
	pc_test();
	rc = gai_rc("localhost", "80", NUMH, AF_UNSPEC, SOCK_STREAM, 0);
	pc_check(rc == EAI_NONAME, "a host name with AI_NUMERICHOST gave %d (%s), expected EAI_NONAME", rc, gai_strerror(rc));
	pc_pass();
}
