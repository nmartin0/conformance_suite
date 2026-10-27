/*
 * With neither node nor service the call fails with EAI_NONAME.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getaddrinfo.026
 */
#include "ga.h"

int main(void)
{
	int rc;

	pc_begin("XSH.getaddrinfo.026");
	pc_test();
	rc = gai_rc(NULL, NULL, 0, AF_UNSPEC, 0, 0);
	pc_check(rc == EAI_NONAME, "two null names gave %d (%s), expected EAI_NONAME", rc, gai_strerror(rc));
	pc_pass();
}
