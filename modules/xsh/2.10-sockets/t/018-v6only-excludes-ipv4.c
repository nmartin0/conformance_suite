/*
 * With IPV6_V6ONLY on, an AF_INET6 listener is reached by IPv6 clients
 * only.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.10-sockets.018
 *
 * An AF_INET6 socket with IPV6_V6ONLY on is bound to the wildcard address
 * and listens. A client using the IPv6 loopback address connects (this
 * shows the listener works; where the IPv6 loopback address is not usable
 * the test is not in use). A client using IPv4 to the same port on
 * 127.0.0.1 gets no connection.
 */
#include "pcsk6.h"

int main(void)
{
	struct sockaddr_in6 a, got;
	struct sockaddr_in a4;
	socklen_t gl = sizeof got;
	int l6, c6, c4, r;

	pc_begin("XSH.2.10-sockets.018");
	guard(60);
	l6 = sock6(SOCK_STREAM);
	v6only(l6, 1);
	memset(&a, 0, sizeof a);
	a.sin6_family = AF_INET6;
	a.sin6_addr = in6addr_any;
	NEED(bind(l6, (struct sockaddr *)&a, sizeof a) == 0, "bind failed: %s", pc_errname(errno));
	NEED(listen(l6, 4) == 0, "listen failed: %s", pc_errname(errno));
	NEED(getsockname(l6, (struct sockaddr *)&got, &gl) == 0, "getsockname failed: %s", pc_errname(errno));
	c6 = socket(AF_INET6, SOCK_STREAM, 0);
	NEED(c6 != -1, "socket failed: %s", pc_errname(errno));
	a.sin6_addr = in6addr_loopback;
	a.sin6_port = got.sin6_port;
	if (connect(c6, (struct sockaddr *)&a, sizeof a) != 0)
		pc_notinuse("the IPv6 loopback address cannot be used to connect (%s)", pc_errname(errno));
	c4 = socket(AF_INET, SOCK_STREAM, 0);
	NEED(c4 != -1, "socket failed: %s", pc_errname(errno));
	memset(&a4, 0, sizeof a4);
	a4.sin_family = AF_INET;
	a4.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	a4.sin_port = got.sin6_port;
	pc_test();
	r = connect(c4, (struct sockaddr *)&a4, sizeof a4);
	pc_check(r == -1, "an IPv4 client connected to an AF_INET6 listener that has IPV6_V6ONLY on");
	pc_pass();
}
