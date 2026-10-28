/*
 * pcsk6.h - IPv6 helpers for the tests of XSH 2.10.20.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 */
#ifndef PCSK6_H
#define PCSK6_H

#include "pcsk.h"

/* Fills *a with the IPv4-mapped form of 127.0.0.1 and the given port (host order). */
static inline void mapped_loopback(struct sockaddr_in6 *a, unsigned short port)
{
	unsigned char *b;

	memset(a, 0, sizeof *a);
	a->sin6_family = AF_INET6;
	a->sin6_port = htons(port);
	b = (unsigned char *)&a->sin6_addr;
	b[10] = 0xff;
	b[11] = 0xff;
	b[12] = 127;
	b[15] = 1;
}

static inline int is_mapped_loopback(const struct in6_addr *x)
{
	static const unsigned char want[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xff, 0xff, 127, 0, 0, 1 };

	return memcmp(x, want, 16) == 0;
}

/* An AF_INET6 socket of the given type; not in use where the family is not supported. */
static inline int sock6(int type)
{
	int s = socket(AF_INET6, type, 0);

	if (s == -1 && (errno == EAFNOSUPPORT || errno == EPROTONOSUPPORT))
		pc_notinuse("AF_INET6 sockets are not supported (%s)", pc_errname(errno));
	NEED(s != -1, "socket(AF_INET6) failed: %s", pc_errname(errno));
	return s;
}

/* Turns IPV6_V6ONLY off (or on). */
static inline void v6only(int s, int on)
{
	NEED(setsockopt(s, IPPROTO_IPV6, IPV6_V6ONLY, &on, sizeof on) == 0, "setsockopt(IPV6_V6ONLY) failed: %s", pc_errname(errno));
}

/* Binds s to the mapped loopback address on an ephemeral port; returns the port (host order). */
static inline unsigned short bind_mapped(int s)
{
	struct sockaddr_in6 a, got;
	socklen_t l = sizeof got;

	mapped_loopback(&a, 0);
	NEED(bind(s, (struct sockaddr *)&a, sizeof a) == 0, "bind() of an AF_INET6 socket to the IPv4-mapped address of 127.0.0.1 failed: %s", pc_errname(errno));
	NEED(getsockname(s, (struct sockaddr *)&got, &l) == 0, "getsockname failed: %s", pc_errname(errno));
	NEED(got.sin6_port != 0, "the port is 0 after bind");
	return ntohs(got.sin6_port);
}

#endif
