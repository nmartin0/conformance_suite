/*
 * Socket creation flags added to the socket type make the call fail with EAI_SOCKTYPE.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getaddrinfo.025
 */
#include "ga.h"

int main(void)
{
	int rc;

	pc_begin("XSH.getaddrinfo.025");
	pc_test();
	rc = gai_rc("127.0.0.1", "80", NUMH, AF_INET, SOCK_STREAM | SOCK_CLOEXEC, 0);
	pc_check(rc == EAI_SOCKTYPE, "SOCK_STREAM|SOCK_CLOEXEC gave %d (%s), expected EAI_SOCKTYPE", rc, gai_strerror(rc));
	pc_pass();
}
