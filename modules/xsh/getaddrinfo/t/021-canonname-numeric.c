/*
 * AI_CANONNAME with a numeric node leaves the node string as the canonical name.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getaddrinfo.021
 */
#include "ga.h"

int main(void)
{
	struct addrinfo *res;

	pc_begin("XSH.getaddrinfo.021");
	pc_test();
	res = gai_ok("127.0.0.1", "80", NUMH | AI_CANONNAME, AF_INET, SOCK_STREAM, 0);
	pc_check(res->ai_canonname != NULL, "ai_canonname of the first entry is null");
	pc_check(strcmp(res->ai_canonname, "127.0.0.1") == 0, "ai_canonname is \"%s\", expected \"127.0.0.1\"", res->ai_canonname);
	freeaddrinfo(res);
	pc_pass();
}
