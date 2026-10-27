/*
 * An entry for a numeric node can be connected to a listener on this host.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getaddrinfo.033
 */
#include "ga.h"

int main(void)
{
	struct addrinfo *res;
	unsigned port;
	char ps[16];
	int lfd, cfd, afd;

	pc_begin("XSH.getaddrinfo.033");
	lfd = listener4(&port);
	snprintf(ps, sizeof ps, "%u", port);
	pc_test();
	res = gai_ok("127.0.0.1", ps, NUMH, AF_INET, SOCK_STREAM, 0);
	cfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	pc_check(cfd >= 0, "socket() failed: %s", pc_errname(errno));
	pc_check(connect(cfd, res->ai_addr, res->ai_addrlen) == 0, "connect() failed: %s", pc_errname(errno));
	afd = accept(lfd, NULL, NULL);
	pc_check(afd >= 0, "accept() found no connection: %s", pc_errname(errno));
	freeaddrinfo(res);
	pc_pass();
}
