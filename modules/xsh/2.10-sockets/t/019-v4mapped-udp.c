/*
 * UDP between an AF_INET6 socket and an IPv4 peer through IPv4-mapped
 * addresses, with the sender's address returned in mapped form.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.10-sockets.019
 *
 * (1) An AF_INET UDP socket sends a datagram to an AF_INET6 UDP socket
 * bound to the mapped form of 127.0.0.1; recvfrom() returns the sender as
 * a sockaddr_in6 with the mapped address and the sender's port. (2) An
 * AF_INET6 UDP socket sends a datagram to the mapped form of 127.0.0.1 and
 * the port of an AF_INET UDP socket, which receives it.
 */
#include "pcsk6.h"

static int udp4(struct sockaddr_in *a)
{
	socklen_t l = sizeof *a;
	int s = socket(AF_INET, SOCK_DGRAM, 0);

	NEED(s != -1, "socket failed: %s", pc_errname(errno));
	memset(a, 0, sizeof *a);
	a->sin_family = AF_INET;
	a->sin_addr.s_addr = pc_loopback4();
	NEED(bind(s, (struct sockaddr *)a, sizeof *a) == 0, "bind failed: %s", pc_errname(errno));
	NEED(getsockname(s, (struct sockaddr *)a, &l) == 0, "getsockname failed: %s", pc_errname(errno));
	return s;
}

int main(void)
{
	struct sockaddr_in s4a, r4a, dst4;
	struct sockaddr_in6 dst, from;
	socklen_t fl = sizeof from, f4 = sizeof dst4;
	int r6, s4, r4, s6;
	unsigned short p6;
	char b[8];
	ssize_t n;

	pc_begin("XSH.2.10-sockets.019");
	guard(60);
	r6 = sock6(SOCK_DGRAM);
	v6only(r6, 0);
	p6 = bind_mapped(r6);
	s4 = udp4(&s4a);
	r4 = udp4(&r4a);
	s6 = sock6(SOCK_DGRAM);
	v6only(s6, 0);
	pc_test();

	memset(&dst4, 0, sizeof dst4);
	dst4.sin_family = AF_INET;
	dst4.sin_addr.s_addr = pc_loopback4();
	dst4.sin_port = htons(p6);
	pc_check(sendto(s4, "hi", 2, 0, (struct sockaddr *)&dst4, sizeof dst4) == 2, "sendto() from the AF_INET socket failed: %s", pc_errname(errno));
	NEED(wait_fd(r6, POLLIN, 5000) > 0, "the datagram did not reach the AF_INET6 socket");
	memset(&from, 0, sizeof from);
	n = recvfrom(r6, b, sizeof b, 0, (struct sockaddr *)&from, &fl);
	pc_check(n == 2 && memcmp(b, "hi", 2) == 0, "recvfrom() on the AF_INET6 socket returned %ld", (long)n);
	pc_check(fl >= sizeof from && from.sin6_family == AF_INET6, "recvfrom() did not return the IPv4 sender's address as an AF_INET6 address");
	pc_check(is_mapped_loopback(&from.sin6_addr), "recvfrom() did not return the IPv4 sender's address in IPv4-mapped form");
	pc_check(from.sin6_port == s4a.sin_port, "recvfrom() returned a port other than the sender's");

	mapped_loopback(&dst, ntohs(r4a.sin_port));
	pc_check(sendto(s6, "yo", 2, 0, (struct sockaddr *)&dst, sizeof dst) == 2, "sendto() from the AF_INET6 socket to a mapped address failed: %s", pc_errname(errno));
	NEED(wait_fd(r4, POLLIN, 5000) > 0, "the datagram did not reach the AF_INET socket");
	n = recvfrom(r4, b, sizeof b, 0, (struct sockaddr *)&dst4, &f4);
	pc_check(n == 2 && memcmp(b, "yo", 2) == 0, "recvfrom() on the AF_INET socket returned %ld", (long)n);
	pc_pass();
}
