/*
 * freeaddrinfo() of a whole list returned by getaddrinfo() leaves errno unchanged.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getaddrinfo.031
 */
#include "ga.h"

int main(void)
{
	struct addrinfo *res;

	pc_begin("XSH.getaddrinfo.031");
	res = gai_ok("127.0.0.1", "80", NUMH, AF_INET, 0, 0);
	pc_test();
	errno = EDOM;
	freeaddrinfo(res);
	pc_check(errno == EDOM, "errno changed to %s", pc_errname(errno));
	pc_pass();
}
