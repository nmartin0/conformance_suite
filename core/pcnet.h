/*
 * pcnet.h - the IPv4 loopback address, for socket tests.
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * POSIX does not define INADDR_LOOPBACK (<netinet/in.h> names it only as
 * a common extension). The one portable way to get the loopback address
 * is getaddrinfo() with a null node name and AI_PASSIVE clear, which the
 * standard says yields it. pc_loopback4() returns that address in network
 * byte order, ready for sin_addr.s_addr; if the system gives no IPv4
 * loopback address the test is UNRESOLVED.
 */
#ifndef PCNET_H
#define PCNET_H

#include "pc.h"
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>

static inline in_addr_t pc_loopback4(void)
{
	static in_addr_t addr;
	static int have;

	if (!have) {
		struct addrinfo hints, *res = NULL;
		int e;

		memset(&hints, 0, sizeof hints);
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		e = getaddrinfo(NULL, "1", &hints, &res);
		if (e != 0 || res == NULL || res->ai_family != AF_INET ||
		    res->ai_addr == NULL)
			pc_unresolved("no IPv4 loopback address: getaddrinfo() "
			    "with a null node name failed (%d)", e);
		memcpy(&addr, &((struct sockaddr_in *)(void *)res->ai_addr)->
		    sin_addr.s_addr, sizeof addr);
		freeaddrinfo(res);
		have = 1;
	}
	return addr;
}

#endif
