/*
 * With AF_INET an IPv6 address string yields no result.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getaddrinfo.009
 */
#include "ga.h"

int main(void)
{
	int rc;

	pc_begin("XSH.getaddrinfo.009");
	pc_test();
	rc = gai_rc("::1", "80", NUMH, AF_INET, SOCK_STREAM, 0);
	pc_check(rc != 0, "AF_INET with an IPv6 address string succeeded");
	pc_pass();
}
