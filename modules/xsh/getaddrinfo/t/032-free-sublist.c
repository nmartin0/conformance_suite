/*
 * freeaddrinfo() accepts a sublist that starts after the first entry and leaves errno unchanged.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getaddrinfo.032
 */
#include "ga.h"

int main(void)
{
	struct addrinfo *res;

	pc_begin("XSH.getaddrinfo.032");
	res = gai_ok("127.0.0.1", "80", NUMH, AF_INET, 0, 0);
	if (res->ai_next == NULL)
		pc_notinuse("the list has a single entry, so there is no sublist after the first");
	pc_test();
	errno = EDOM;
	freeaddrinfo(res->ai_next);
	pc_check(errno == EDOM, "errno changed to %s", pc_errname(errno));
	pc_pass();
}
