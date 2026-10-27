/*
 * With AF_INET6 and no AI_V4MAPPED an IPv4 address string yields no result.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getaddrinfo.006
 */
#include "ga.h"

int main(void)
{
	int rc;

	pc_begin("XSH.getaddrinfo.006");
	need_v6();
	pc_test();
	rc = gai_rc("127.0.0.1", "80", NUMH, AF_INET6, SOCK_STREAM, 0);
	pc_check(rc != 0, "AF_INET6 with an IPv4 address string succeeded");
	pc_pass();
}
