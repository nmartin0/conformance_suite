/*
 * With AF_UNSPEC, AI_PASSIVE and no node, each entry holds the wildcard address of its family.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getaddrinfo.015
 */
#include "ga.h"

int main(void)
{
	struct addrinfo *res, *p;

	pc_begin("XSH.getaddrinfo.015");
	pc_test();
	res = gai_ok(NULL, "9", AI_PASSIVE | AI_NUMERICSERV, AF_UNSPEC, SOCK_STREAM, 0);
	for (p = res; p != NULL; p = p->ai_next) {
		if (p->ai_family == AF_INET) {
			pc_check(is_wild4(sin_of(p)), "an AF_INET entry is not INADDR_ANY");
		} else {
			pc_check(p->ai_family == AF_INET6, "entry family %d is neither AF_INET nor AF_INET6", p->ai_family);
			pc_check(IN6_IS_ADDR_UNSPECIFIED(&sin6_of(p)->sin6_addr), "an AF_INET6 entry is not the wildcard address");
		}
	}
	freeaddrinfo(res);
	pc_pass();
}
