/*
 * An unrecognized address family fails with EAI_FAMILY.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getaddrinfo.023
 */
#include "ga.h"

int main(void)
{
	int rc;

	pc_begin("XSH.getaddrinfo.023");
	pc_test();
	rc = gai_rc("127.0.0.1", "80", NUMH, -1, SOCK_STREAM, 0);
	pc_check(rc == EAI_FAMILY, "family -1 gave %d (%s), expected EAI_FAMILY", rc, gai_strerror(rc));
	pc_pass();
}
