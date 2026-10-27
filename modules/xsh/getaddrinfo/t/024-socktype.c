/*
 * An unrecognized socket type fails with EAI_SOCKTYPE.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getaddrinfo.024
 */
#include "ga.h"

int main(void)
{
	int rc;

	pc_begin("XSH.getaddrinfo.024");
	pc_test();
	rc = gai_rc("127.0.0.1", "80", NUMH, AF_INET, -1, 0);
	pc_check(rc == EAI_SOCKTYPE, "socket type -1 gave %d (%s), expected EAI_SOCKTYPE", rc, gai_strerror(rc));
	pc_pass();
}
