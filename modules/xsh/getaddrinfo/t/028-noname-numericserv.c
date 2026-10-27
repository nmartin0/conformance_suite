/*
 * With AI_NUMERICSERV a service that is not a number fails with EAI_NONAME.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getaddrinfo.028
 */
#include "ga.h"

int main(void)
{
	int rc;

	pc_begin("XSH.getaddrinfo.028");
	pc_test();
	rc = gai_rc("127.0.0.1", "http", NUMH, AF_INET, SOCK_STREAM, 0);
	pc_check(rc == EAI_NONAME, "a service name with AI_NUMERICSERV gave %d (%s), expected EAI_NONAME", rc, gai_strerror(rc));
	pc_pass();
}
