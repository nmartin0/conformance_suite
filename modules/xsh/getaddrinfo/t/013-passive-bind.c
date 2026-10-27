/*
 * Entries from AI_PASSIVE with no node can be bound with their ai_addr and ai_addrlen.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getaddrinfo.013
 */
#include "ga.h"

int main(void)
{
	struct addrinfo *res, *p;

	pc_begin("XSH.getaddrinfo.013");
	pc_test();
	res = gai_ok(NULL, "0", AI_PASSIVE | AI_NUMERICSERV, AF_INET, SOCK_STREAM, 0);
	for (p = res; p != NULL; p = p->ai_next) {
		int fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);

		pc_check(fd >= 0, "socket() failed: %s", pc_errname(errno));
		pc_check(bind(fd, p->ai_addr, p->ai_addrlen) == 0, "bind() failed: %s", pc_errname(errno));
		(void)close(fd);
	}
	freeaddrinfo(res);
	pc_pass();
}
