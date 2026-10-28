/*
 * An AF_INET6 socket can be bound to the IPv4-mapped form of a node's IPv4
 * address, and getsockname() then returns that mapped address.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.10-sockets.016
 *
 * The node's IPv4 address is the loopback address 127.0.0.1 (the test is
 * not in use if an AF_INET socket cannot be bound to it). IPV6_V6ONLY is
 * turned off.
 */
#include "pcsk6.h"

int main(void)
{
	struct sockaddr_in a4;
	struct sockaddr_in6 a, got;
	socklen_t l;
	int s4, s6;

	pc_begin("XSH.2.10-sockets.016");
	guard(60);
	s4 = socket(AF_INET, SOCK_STREAM, 0);
	NEED(s4 != -1, "socket failed: %s", pc_errname(errno));
	memset(&a4, 0, sizeof a4);
	a4.sin_family = AF_INET;
	a4.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	if (bind(s4, (struct sockaddr *)&a4, sizeof a4) != 0)
		pc_notinuse("the node has no IPv4 loopback address (bind: %s)", pc_errname(errno));
	(void)close(s4);
	s6 = sock6(SOCK_STREAM);
	v6only(s6, 0);
	pc_test();
	mapped_loopback(&a, 0);
	pc_check(bind(s6, (struct sockaddr *)&a, sizeof a) == 0, "bind() of an AF_INET6 socket to the IPv4-mapped address of 127.0.0.1 failed: %s", pc_errname(errno));
	memset(&got, 0, sizeof got);
	l = sizeof got;
	pc_check(getsockname(s6, (struct sockaddr *)&got, &l) == 0, "getsockname() failed: %s", pc_errname(errno));
	pc_check(l >= sizeof got, "getsockname() length %lu is smaller than a sockaddr_in6", (unsigned long)l);
	pc_check(got.sin6_family == AF_INET6, "the family of the name is not AF_INET6");
	pc_check(is_mapped_loopback(&got.sin6_addr), "getsockname() did not return the IPv4-mapped form of 127.0.0.1");
	pc_check(got.sin6_port != 0, "the port of the name is 0");
	pc_pass();
}
