/*
 * TCP between an AF_INET6 socket and an IPv4 peer through IPv4-mapped
 * addresses, with the peer's address returned in mapped form.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.10-sockets.017
 *
 * (1) An AF_INET6 client connects to the mapped form of 127.0.0.1 and the
 * port of an AF_INET listener; the connection works and getpeername() of
 * the client gives the mapped address and the port. (2) An AF_INET6
 * listener bound to the mapped address accepts a connection from an
 * AF_INET client; accept() returns the client's address as a sockaddr_in6
 * holding the mapped form of 127.0.0.1 and the client's port.
 */
#include "pcsk6.h"

int main(void)
{
	struct sockaddr_in l4, c4a;
	struct sockaddr_in6 a, got;
	socklen_t gl, cl;
	int l, c6, srv, l6, c4, acc;
	unsigned short p6;
	char b[4];

	pc_begin("XSH.2.10-sockets.017");
	guard(60);
	l = tcp_listener(&l4);
	c6 = sock6(SOCK_STREAM);
	v6only(c6, 0);
	pc_test();

	mapped_loopback(&a, ntohs(l4.sin_port));
	pc_check(connect(c6, (struct sockaddr *)&a, sizeof a) == 0, "connect() of an AF_INET6 socket to the mapped address of a local IPv4 listener failed: %s", pc_errname(errno));
	NEED(wait_fd(l, POLLIN, 5000) > 0, "the connection did not reach the IPv4 listener");
	srv = accept(l, NULL, NULL);
	NEED(srv != -1, "accept failed: %s", pc_errname(errno));
	NEED(send(c6, "hi", 2, MSG_NOSIGNAL) == 2, "send failed: %s", pc_errname(errno));
	pc_check(recv_n(srv, b, 2) == 2 && memcmp(b, "hi", 2) == 0, "data sent through the mapped connection did not arrive");
	memset(&got, 0, sizeof got);
	gl = sizeof got;
	pc_check(getpeername(c6, (struct sockaddr *)&got, &gl) == 0, "getpeername() failed: %s", pc_errname(errno));
	pc_check(got.sin6_family == AF_INET6 && is_mapped_loopback(&got.sin6_addr) && got.sin6_port == l4.sin_port,
	    "getpeername() of the AF_INET6 client did not give the mapped address and port of the IPv4 listener");

	l6 = sock6(SOCK_STREAM);
	v6only(l6, 0);
	p6 = bind_mapped(l6);
	NEED(listen(l6, 2) == 0, "listen failed: %s", pc_errname(errno));
	c4 = socket(AF_INET, SOCK_STREAM, 0);
	NEED(c4 != -1, "socket failed: %s", pc_errname(errno));
	memset(&l4, 0, sizeof l4);
	l4.sin_family = AF_INET;
	l4.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	l4.sin_port = htons(p6);
	pc_check(connect(c4, (struct sockaddr *)&l4, sizeof l4) == 0, "an IPv4 client could not connect to an AF_INET6 listener bound to the mapped address: %s", pc_errname(errno));
	cl = sizeof c4a;
	NEED(getsockname(c4, (struct sockaddr *)&c4a, &cl) == 0, "getsockname failed: %s", pc_errname(errno));
	NEED(wait_fd(l6, POLLIN, 5000) > 0, "the connection did not reach the AF_INET6 listener");
	memset(&got, 0, sizeof got);
	gl = sizeof got;
	acc = accept(l6, (struct sockaddr *)&got, &gl);
	pc_check(acc != -1, "accept() failed: %s", pc_errname(errno));
	pc_check(gl >= sizeof got, "accept() length %lu is smaller than a sockaddr_in6", (unsigned long)gl);
	pc_check(got.sin6_family == AF_INET6, "accept() did not return the IPv4 peer's address as an AF_INET6 address");
	pc_check(is_mapped_loopback(&got.sin6_addr), "accept() did not return the IPv4 peer's address in IPv4-mapped form");
	pc_check(got.sin6_port == c4a.sin_port, "accept() returned a port other than the client's");
	pc_pass();
}
